#ifndef GLIB_VECTOR_H
#define GLIB_VECTOR_H

#include <cstddef>
#include <new>
#include <stdexcept>
#include <utility>

namespace GLib::DS {

  template <typename T> class Vector {

    inline static constexpr std::size_t InitCapacity = 32;

    T* pData{nullptr};
    std::size_t capacity{0};
    std::size_t size{0};

    void Resize();

  public:

    Vector(const std::size_t initCapacity = InitCapacity);
    ~Vector();

    Vector(Vector&& other) noexcept;
    Vector(const Vector& other) noexcept;
    Vector& operator=(const Vector& other);
    Vector& operator=(Vector&& other) noexcept;
    T& operator[](const std::size_t index);

    void Push(const T& t);
    std::size_t Size() const;
    std::size_t Capacity() const;
    void Clear();

  };

} // namespace GLib::DS

#include "Vector.tpp"
#endif
