#ifndef G_GENERAL_HEAP_ALLOCATOR_H
#define G_GENERAL_HEAP_ALLOCATOR_H

#include <cstddef>
#include <memory>
#include <mutex>
#include <stdexcept>
#include <new>

namespace GLib::Core {

// Unit for specifying allocator capacity.
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

static constexpr size_t ALIGNMENT  = alignof(std::max_align_t);
static constexpr size_t HEADER_SIZE = AlignUp(sizeof(BlockHeader), ALIGNMENT);

static constexpr size_t MIN_BLOCK_SIZE = HEADER_SIZE + ALIGNMENT;

// The raw memory pool. Shared (via shared_ptr) across all rebound copies of
// GeneralHeapAllocator so every rebind variant draws from the same arena.
class MemoryPool {
public:
    MemoryPool(MemoryUnit unit, size_t count)
        : m_Capacity(ToBytes(unit, count))
    {
        if (m_Capacity == 0)
            throw std::invalid_argument("GeneralHeapAllocator: capacity must be > 0");

        m_Base = new char[m_Capacity];

        // Bootstrap: the entire pool is one big free block.
        auto* header   = reinterpret_cast<BlockHeader*>(m_Base);
        header->size      = m_Capacity;
        header->in_use    = false;
        header->next_free = nullptr;
        m_FreeList = header;
    }

    ~MemoryPool() { delete[] m_Base; }

    MemoryPool(const MemoryPool&)            = delete;
    MemoryPool& operator=(const MemoryPool&) = delete;

    // Allocate at least `bytes` bytes, aligned to ALIGNMENT.
    void* Allocate(size_t bytes) {
        std::lock_guard<std::mutex> lock(m_Mutex);

        const size_t needed = HEADER_SIZE + AlignUp(bytes, ALIGNMENT);

        BlockHeader** prev = &m_FreeList;
        BlockHeader*  curr = m_FreeList;

        while (curr) {
            if (curr->size >= needed) {
                // Split the block if the leftover is large enough to be useful.
                if (curr->size >= needed + MIN_BLOCK_SIZE) {
                    auto* split      = reinterpret_cast<BlockHeader*>(
                                           reinterpret_cast<char*>(curr) + needed);
                    split->size      = curr->size - needed;
                    split->in_use    = false;
                    split->next_free = curr->next_free;
                    curr->size       = needed;
                    *prev            = split;
                } else {
                    // Use the whole block; remove from free list.
                    *prev = curr->next_free;
                }

                curr->in_use    = true;
                curr->next_free = nullptr;
                return reinterpret_cast<char*>(curr) + HEADER_SIZE;
            }

            prev = &curr->next_free;
            curr = curr->next_free;
        }

        throw std::bad_alloc();
    }

    void Deallocate(void* p) noexcept {
        if (!p) return;

        std::lock_guard<std::mutex> lock(m_Mutex);

        auto* header  = reinterpret_cast<BlockHeader*>(
                            static_cast<char*>(p) - HEADER_SIZE);
        header->in_use = false;

        // Insert into the free list sorted by address so adjacent blocks
        // are always neighbours in the list — makes coalescing O(1).
        BlockHeader*  prev_block = nullptr;
        BlockHeader** prev_ptr   = &m_FreeList;
        BlockHeader*  curr       = m_FreeList;

        while (curr && curr < header) {
            prev_block = curr;
            prev_ptr   = &curr->next_free;
            curr       = curr->next_free;
        }

        header->next_free = curr;
        *prev_ptr         = header;

        // Coalesce with the next block if it is physically adjacent.
        char* header_end = reinterpret_cast<char*>(header) + header->size;
        if (header->next_free &&
            reinterpret_cast<char*>(header->next_free) == header_end) {
            header->size     += header->next_free->size;
            header->next_free = header->next_free->next_free;
        }

        // Coalesce with the previous block if it is physically adjacent.
        if (prev_block) {
            char* prev_end = reinterpret_cast<char*>(prev_block) + prev_block->size;
            if (prev_end == reinterpret_cast<char*>(header)) {
                prev_block->size     += header->size;
                prev_block->next_free = header->next_free;
            }
        }
    }

    size_t Capacity() const noexcept { return m_Capacity; }

private:
    char*        m_Base;
    size_t       m_Capacity;
    BlockHeader* m_FreeList;
    std::mutex   m_Mutex;
};

} // namespace detail

// ---------------------------------------------------------------------------
// GeneralHeapAllocator<T>
//
// STL-compatible allocator backed by a fixed-capacity pool.
//
// Usage:
//   GeneralHeapAllocator<int> gha(MemoryUnit::MB, 4); // 4 MB pool
//   std::vector<int, GeneralHeapAllocator<int>> v(gha);
// ---------------------------------------------------------------------------
template<typename T>
class GeneralHeapAllocator {
public:
    using value_type      = T;
    using pointer         = T*;
    using const_pointer   = const T*;
    using size_type       = std::size_t;
    using difference_type = std::ptrdiff_t;

    // STL containers call rebind<U>::other to get an allocator for their
    // internal node types. The rebound allocator shares the same pool.
    template<typename U>
    struct rebind {
        using other = GeneralHeapAllocator<U>;
    };

    // Primary constructor — creates the underlying pool.
    GeneralHeapAllocator(MemoryUnit unit, size_type count)
        : m_Pool(std::make_shared<detail::MemoryPool>(unit, count))
    {}

    // Copying shares the pool (same arena, different type parameter).
    GeneralHeapAllocator(const GeneralHeapAllocator&)            = default;
    GeneralHeapAllocator& operator=(const GeneralHeapAllocator&) = default;

    // Rebind copy constructor — called by STL when it needs a different T.
    template<typename U>
    explicit GeneralHeapAllocator(const GeneralHeapAllocator<U>& other) noexcept
        : m_Pool(other.m_Pool)
    {}

    // Allocate storage for n objects of type T.
    T* allocate(size_type n) {
        return static_cast<T*>(m_Pool->Allocate(n * sizeof(T)));
    }

    // Return storage for n objects back to the pool.
    void deallocate(T* p, size_type /*n*/) noexcept {
        m_Pool->Deallocate(static_cast<void*>(p));
    }

    size_type max_size() const noexcept {
        return m_Pool->Capacity() / sizeof(T);
    }

    // Two allocators are equal if they share the same pool.
    template<typename U>
    bool operator==(const GeneralHeapAllocator<U>& other) const noexcept {
        return m_Pool == other.m_Pool;
    }

    template<typename U>
    bool operator!=(const GeneralHeapAllocator<U>& other) const noexcept {
        return m_Pool != other.m_Pool;
    }

private:
    std::shared_ptr<detail::MemoryPool> m_Pool;

    // All rebind instantiations need access to m_Pool.
    template<typename U> friend class GeneralHeapAllocator;
};

} // namespace GLib::Core

#endif // G_GENERAL_HEAP_ALLOCATOR_H
