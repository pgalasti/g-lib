#ifndef G_GENERAL_HEAP_ALLOCATOR_H
#define G_GENERAL_HEAP_ALLOCATOR_H

#include <cstddef>
#include <memory>
#include <mutex>
#include <new>
#include <stdexcept>
#include <utility>

namespace GLib::Core {

// Unit for specifying pool capacity.
enum class MemoryUnit : uint8_t {
    KB = 0,
    MB,
    GB
};

namespace detail {

constexpr size_t ToBytes(MemoryUnit unit, size_t count) noexcept {
    switch (unit) {
        case MemoryUnit::KB: return count * 1024ULL;
        case MemoryUnit::MB: return count * 1024ULL * 1024ULL;
        case MemoryUnit::GB: return count * 1024ULL * 1024ULL * 1024ULL;
    }
    return 0;
}

constexpr size_t AlignUp(size_t value, size_t alignment) noexcept {
    return (value + alignment - 1) & ~(alignment - 1);
}

struct BlockHeader {
    size_t       size;      // Total bytes for this block, including this header.
    bool         in_use;
    BlockHeader* next_free; // Intrusive free-list link; only valid when !in_use.
};

static constexpr size_t ALIGNMENT       = alignof(std::max_align_t);
static constexpr size_t HEADER_SIZE     = AlignUp(sizeof(BlockHeader), ALIGNMENT);
static constexpr size_t MIN_BLOCK_SIZE  = HEADER_SIZE + ALIGNMENT;

} // namespace detail

// ---------------------------------------------------------------------------
// MemoryPool
//
// A fixed-capacity arena allocator. Makes one OS allocation at construction
// and manages that memory with a coalescing free list.
//
// Can be used directly for type-safe raw memory or full object lifecycle,
// or handed to GeneralHeapAllocator for use with STL containers.
//
// Usage:
//   MemoryPool pool(MemoryUnit::MB, 4);
//
//   // Raw memory
//   int* arr = pool.Acquire<int>(64);
//   pool.Release(arr, 64);
//
//   // Full object lifecycle
//   MyClass* obj = pool.New<MyClass>(arg1, arg2);
//   pool.Delete(obj);
//
//   // Bulk discard — O(1) regardless of how many objects are live
//   pool.Reset();
// ---------------------------------------------------------------------------
class MemoryPool {
public:
    MemoryPool(MemoryUnit unit, size_t count)
        : m_Capacity(detail::ToBytes(unit, count)), m_BytesUsed(0)
    {
        if (m_Capacity == 0)
            throw std::invalid_argument("MemoryPool: capacity must be > 0");

        m_Base = new char[m_Capacity];
        InitFreeList();
    }

    ~MemoryPool() { delete[] m_Base; }

    MemoryPool(const MemoryPool&)            = delete;
    MemoryPool& operator=(const MemoryPool&) = delete;

    // ── raw typed memory ────────────────────────────────────────────────────

    // Allocate storage for n objects of type T. Does NOT construct them.
    template<typename T>
    T* Acquire(size_t n = 1) {
        return static_cast<T*>(Allocate(n * sizeof(T)));
    }

    // Return storage for n objects of type T back to the pool.
    // Does NOT call destructors — use Delete for that.
    template<typename T>
    void Release(T* p, size_t n = 1) noexcept {
        (void)n;
        Deallocate(static_cast<void*>(p));
    }

    // ── full object lifecycle ───────────────────────────────────────────────

    // Allocate storage and construct a single object in place.
    template<typename T, typename... Args>
    T* New(Args&&... args) {
        T* p = Acquire<T>(1);
        ::new (static_cast<void*>(p)) T(std::forward<Args>(args)...);
        return p;
    }

    // Destruct a single object and return its storage to the pool.
    template<typename T>
    void Delete(T* p) noexcept {
        if (!p) return;
        p->~T();
        Release(p, 1);
    }

    // ── pool management ─────────────────────────────────────────────────────

    // Reset the entire pool to its initial empty state in O(1).
    // All previously acquired pointers are immediately invalidated.
    void Reset() noexcept {
        std::lock_guard<std::mutex> lock(m_Mutex);
        m_BytesUsed = 0;
        InitFreeList();
    }

    // ── diagnostics ─────────────────────────────────────────────────────────

    size_t Capacity()   const noexcept { return m_Capacity; }
    size_t BytesUsed()  const noexcept { return m_BytesUsed; }
    size_t BytesFree()  const noexcept { return m_Capacity - m_BytesUsed; }

    // ── internals used by GeneralHeapAllocator ──────────────────────────────

    void* Allocate(size_t bytes) {
        std::lock_guard<std::mutex> lock(m_Mutex);

        const size_t needed = detail::HEADER_SIZE +
                              detail::AlignUp(bytes, detail::ALIGNMENT);

        detail::BlockHeader** prev = &m_FreeList;
        detail::BlockHeader*  curr = m_FreeList;

        while (curr) {
            if (curr->size >= needed) {
                if (curr->size >= needed + detail::MIN_BLOCK_SIZE) {
                    auto* split      = reinterpret_cast<detail::BlockHeader*>(
                                           reinterpret_cast<char*>(curr) + needed);
                    split->size      = curr->size - needed;
                    split->in_use    = false;
                    split->next_free = curr->next_free;
                    curr->size       = needed;
                    *prev            = split;
                } else {
                    *prev = curr->next_free;
                }

                curr->in_use    = true;
                curr->next_free = nullptr;
                m_BytesUsed    += curr->size;
                return reinterpret_cast<char*>(curr) + detail::HEADER_SIZE;
            }

            prev = &curr->next_free;
            curr = curr->next_free;
        }

        throw std::bad_alloc();
    }

    void Deallocate(void* p) noexcept {
        if (!p) return;

        std::lock_guard<std::mutex> lock(m_Mutex);

        auto* header   = reinterpret_cast<detail::BlockHeader*>(
                             static_cast<char*>(p) - detail::HEADER_SIZE);
        header->in_use = false;
        m_BytesUsed   -= header->size;

        detail::BlockHeader*  prev_block = nullptr;
        detail::BlockHeader** prev_ptr   = &m_FreeList;
        detail::BlockHeader*  curr       = m_FreeList;

        while (curr && curr < header) {
            prev_block = curr;
            prev_ptr   = &curr->next_free;
            curr       = curr->next_free;
        }

        header->next_free = curr;
        *prev_ptr         = header;

        // Coalesce with next neighbour.
        char* header_end = reinterpret_cast<char*>(header) + header->size;
        if (header->next_free &&
            reinterpret_cast<char*>(header->next_free) == header_end) {
            header->size     += header->next_free->size;
            header->next_free = header->next_free->next_free;
        }

        // Coalesce with previous neighbour.
        if (prev_block) {
            char* prev_end = reinterpret_cast<char*>(prev_block) + prev_block->size;
            if (prev_end == reinterpret_cast<char*>(header)) {
                prev_block->size     += header->size;
                prev_block->next_free = header->next_free;
            }
        }
    }

private:
    void InitFreeList() noexcept {
        auto* header      = reinterpret_cast<detail::BlockHeader*>(m_Base);
        header->size      = m_Capacity;
        header->in_use    = false;
        header->next_free = nullptr;
        m_FreeList        = header;
    }

    char*                 m_Base;
    size_t                m_Capacity;
    size_t                m_BytesUsed;
    detail::BlockHeader*  m_FreeList;
    std::mutex            m_Mutex;
};

// ---------------------------------------------------------------------------
// GeneralHeapAllocator<T>
//
// STL-compatible allocator backed by a shared MemoryPool.
//
// Usage:
//   GeneralHeapAllocator<int> gha(MemoryUnit::MB, 4);
//   std::vector<int, GeneralHeapAllocator<int>> v(gha);
//
// The underlying pool can also be accessed directly:
//   auto pool = gha.Pool();
//   int* p = pool->Acquire<int>(10);
// ---------------------------------------------------------------------------
template<typename T>
class GeneralHeapAllocator {
public:
    using value_type      = T;
    using pointer         = T*;
    using const_pointer   = const T*;
    using size_type       = std::size_t;
    using difference_type = std::ptrdiff_t;

    template<typename U>
    struct rebind {
        using other = GeneralHeapAllocator<U>;
    };

    // Primary constructor — creates the underlying pool.
    GeneralHeapAllocator(MemoryUnit unit, size_type count)
        : m_Pool(std::make_shared<MemoryPool>(unit, count))
    {}

    // Wrap an existing pool — lets you share one pool between the allocator
    // and direct Acquire/New calls.
    explicit GeneralHeapAllocator(std::shared_ptr<MemoryPool> pool)
        : m_Pool(std::move(pool))
    {}

    GeneralHeapAllocator(const GeneralHeapAllocator&)            = default;
    GeneralHeapAllocator& operator=(const GeneralHeapAllocator&) = default;

    template<typename U>
    explicit GeneralHeapAllocator(const GeneralHeapAllocator<U>& other) noexcept
        : m_Pool(other.m_Pool)
    {}

    T* allocate(size_type n) {
        return static_cast<T*>(m_Pool->Allocate(n * sizeof(T)));
    }

    void deallocate(T* p, size_type /*n*/) noexcept {
        m_Pool->Deallocate(static_cast<void*>(p));
    }

    size_type max_size() const noexcept {
        return m_Pool->Capacity() / sizeof(T);
    }

    // Direct access to the pool for Acquire/New/Reset etc.
    std::shared_ptr<MemoryPool> Pool() const noexcept { return m_Pool; }

    template<typename U>
    bool operator==(const GeneralHeapAllocator<U>& other) const noexcept {
        return m_Pool == other.m_Pool;
    }

    template<typename U>
    bool operator!=(const GeneralHeapAllocator<U>& other) const noexcept {
        return m_Pool != other.m_Pool;
    }

private:
    std::shared_ptr<MemoryPool> m_Pool;

    template<typename U> friend class GeneralHeapAllocator;
};

} // namespace GLib::Core

#endif // G_GENERAL_HEAP_ALLOCATOR_H
