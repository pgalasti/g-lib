#ifndef GLIB_SMART_POINTERS_H
#define GLIB_SMART_POINTERS_H

namespace GLib::Memory {

// *** Don't use these in production code ***
// 
// Seriously, the std implementations are incredibly good. I'm just writing
// this for fun and practice and not used for arrays.
//
// These are probably slower and defintely take more memory for not using EBO

template <typename T>
struct GenericDeleter {
  void operator()(T* ptr) const { delete ptr; }
};

template <typename T, typename D = GenericDeleter<T>>
class UniquePointer {
  T* ptr     {nullptr};
  D deleter  {};

public:
  UniquePointer() = default;
  explicit UniquePointer(T* ptr) : ptr {ptr} {}
  ~UniquePointer() {
    deleter(ptr);
  }

  UniquePointer(UniquePointer&& otherUp) {
    ptr     = otherUp.ptr;
    deleter = otherUp.deleter;

    otherUp.ptr = nullptr;
  }

  UniquePointer& operator=(UniquePointer&& otherUp) {
    if (this != &otherUp) {
      deleter(ptr);
      ptr     = otherUp.ptr;
      deleter = otherUp.deleter;

      otherUp.ptr = nullptr;
    }

    return *this;
  }

  UniquePointer(const UniquePointer& otherUp)            = delete;
  UniquePointer& operator=(const UniquePointer& otherUp) = delete;

  T& operator*()  const noexcept  { return *ptr;  }

  T* operator->() const noexcept  { return ptr;   }

  T* get()        const noexcept  { return ptr;   }
};

template <typename T, typename D = GenericDeleter<T>>
using UP = UniquePointer<T, D>;


} // Namesapce GLib::Memory

#endif // GLIB_SMART_POINTERS_H guard

