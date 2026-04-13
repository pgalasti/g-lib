namespace GLib::DS {

template <typename T>
Vector<T>::Vector(const std::size_t initCapacity)
    : pData(static_cast<T*>(::operator new(initCapacity * sizeof(T)))),
      capacity(initCapacity),
      size(0)
{}

template <typename T>
Vector<T>::~Vector() {
  Clear();
  ::operator delete(pData);
}

template <typename T>
Vector<T>::Vector(Vector&& other) noexcept
    : pData(other.pData), capacity(other.capacity), size(other.size) {
  other.pData    = nullptr;
  other.capacity = 0;
  other.size     = 0;
}

template <typename T>
Vector<T>::Vector(const Vector& other) noexcept
    : pData(static_cast<T*>(::operator new(other.capacity * sizeof(T)))),
      capacity(other.capacity),
      size(other.size) {
  for (std::size_t i = 0; i < other.size; ++i) {
    ::new(pData + i) T(other.pData[i]);
  }
}

template <typename T>
Vector<T>& Vector<T>::operator=(const Vector& other) {
  if (this == &other) return *this;

  Clear();
  ::operator delete(pData);

  pData    = static_cast<T*>(::operator new(other.capacity * sizeof(T)));
  capacity = other.capacity;
  size     = other.size;

  for (std::size_t i = 0; i < other.size; ++i) {
    ::new(pData + i) T(other.pData[i]);
  }

  return *this;
}

template <typename T>
Vector<T>& Vector<T>::operator=(Vector&& other) noexcept {
  if (this == &other) return *this;

  Clear();
  ::operator delete(pData);

  pData    = other.pData;
  capacity = other.capacity;
  size     = other.size;

  other.pData    = nullptr;
  other.capacity = 0;
  other.size     = 0;

  return *this;
}

template <typename T>
T& Vector<T>::operator[](const std::size_t index) {
  return pData[index];
}

template <typename T>
void Vector<T>::Push(const T& t) {
  if (size == capacity) {
    Resize();
  }
  ::new(pData + size) T(t);
  ++size;
}

template <typename T>
std::size_t Vector<T>::Size() const {
  return size;
}

template <typename T>
std::size_t Vector<T>::Capacity() const {
  return capacity;
}

template <typename T>
void Vector<T>::Clear() {
  for (std::size_t i = 0; i < size; ++i) {
    pData[i].~T();
  }
  size = 0;
}

template <typename T>
void Vector<T>::Resize() {
  const std::size_t newCapacity = capacity * 2;
  T* pNewData = static_cast<T*>(::operator new(newCapacity * sizeof(T)));

  for (std::size_t i = 0; i < size; ++i) {
    ::new(pNewData + i) T(std::move(pData[i]));
    pData[i].~T();
  }

  ::operator delete(pData);
  pData    = pNewData;
  capacity = newCapacity;
}

} // namespace GLib::DS
