#ifndef G_OBJECT_POOL_H
#define G_OBJECT_POOL_H

#include <cassert>
#include <cstddef>
#include <mutex>
#include <new>
#include <stdexcept>
#include <utility>

namespace GLib::Core {

// Mutex policy for single-threaded use — zero overhead, no locking.
struct NoMutex {
    void lock()   noexcept {}
    void unlock() noexcept {}
};

// ---------------------------------------------------------------------------
// ObjectPool<T, Mutex>
//
// A fixed-capacity pool dedicated to a single type. Each slot is exactly
// sizeof(T) bytes — free slots store the intrusive free-list pointer inside
// themselves so there is zero per-slot overhead. Acquire and Release are O(1).
//
// The Mutex policy controls thread safety:
//   ObjectPool<T>            — thread-safe (std::mutex, default)
//   ObjectPool<T, NoMutex>   — single-threaded, zero locking overhead
//
// Usage:
//   ObjectPool<MyClass> pool(64);            // thread-safe, 64 slots
//   ObjectPool<MyClass, NoMutex> pool(64);   // single-threaded
//
//   // Raw memory (no construction)
//   MyClass* raw = pool.Acquire();
//   pool.Release(raw);
//
//   // Full object lifecycle
//   MyClass* obj = pool.New(arg1, arg2);
//   pool.Delete(obj);
// ---------------------------------------------------------------------------
template<typename T, typename Mutex = std::mutex>
class ObjectPool {
public:
    explicit ObjectPool(size_t capacity)
        : m_Capacity(capacity), m_Available(capacity)
    {
        if (capacity == 0)
            throw std::invalid_argument("ObjectPool: capacity must be > 0");

        m_Slots = static_cast<Slot*>(::operator new(capacity * sizeof(Slot)));

        // Link every slot into the free list.
        for (size_t i = 0; i < capacity - 1; ++i)
            m_Slots[i].next = &m_Slots[i + 1];
        m_Slots[capacity - 1].next = nullptr;

        m_FreeList = &m_Slots[0];
    }

    ~ObjectPool() {
        ::operator delete(m_Slots);
    }

    ObjectPool(const ObjectPool&)            = delete;
    ObjectPool& operator=(const ObjectPool&) = delete;

    // ── raw memory ──────────────────────────────────────────────────────────

    // Pop a slot from the free list. Does NOT construct T.
    T* Acquire() {
        std::lock_guard<Mutex> lock(m_Mutex);

        if (!m_FreeList)
            throw std::bad_alloc();

        Slot* slot = m_FreeList;
        m_FreeList = slot->next;
        --m_Available;

        return reinterpret_cast<T*>(slot);
    }

    // Push a slot back onto the free list. Does NOT destruct T.
    void Release(T* p) noexcept {
        if (!p) return;

        std::lock_guard<Mutex> lock(m_Mutex);

        assert(Owns(p) && "ObjectPool::Release: pointer not from this pool");

        Slot* slot     = reinterpret_cast<Slot*>(p);
        slot->next     = m_FreeList;
        m_FreeList     = slot;
        ++m_Available;
    }

    // ── full object lifecycle ───────────────────────────────────────────────

    // Pop a slot and construct T in place.
    template<typename... Args>
    T* New(Args&&... args) {
        T* p = Acquire();
        ::new (static_cast<void*>(p)) T(std::forward<Args>(args)...);
        return p;
    }

    // Destruct T and push the slot back onto the free list.
    void Delete(T* p) noexcept {
        if (!p) return;
        p->~T();
        Release(p);
    }

    // ── diagnostics ─────────────────────────────────────────────────────────

    size_t Capacity()  const noexcept { return m_Capacity; }
    size_t Available() const noexcept { return m_Available; }
    size_t InUse()     const noexcept { return m_Capacity - m_Available; }

    // Returns true if p was acquired from this pool.
    bool Owns(const T* p) const noexcept {
        const auto* slot = reinterpret_cast<const Slot*>(p);
        return slot >= m_Slots && slot < m_Slots + m_Capacity;
    }

private:
    // A slot is either live T storage or a free-list node — never both.
    // Storing the next pointer inside the free slot costs nothing.
    union Slot {
        alignas(T) char storage[sizeof(T)];
        Slot* next;
    };

    Slot*      m_Slots;
    Slot*      m_FreeList;
    size_t     m_Capacity;
    size_t     m_Available;
    Mutex      m_Mutex;
};

} // namespace GLib::Core

#endif // G_OBJECT_POOL_H
