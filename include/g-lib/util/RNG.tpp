#include <stdexcept>

namespace GLib::Util {

template <typename T>
RNG<T>::RNG(T min, T max)
    : m_Range{min, max},
      m_Engine(std::random_device{}()),
      m_Distribution(min, max) {}

template <typename T>
RNG<T>::RNG(const Range &range) : RNG(range.min, range.max) {}

template <typename T>
T RNG<T>::Generate() {
  return m_Distribution(m_Engine);
}

template <typename T>
void RNG<T>::Fill(unsigned int count) {
  for (unsigned int i = 0; i < count; ++i) {
    m_Queue.Push(Generate());
  }
}

template <typename T>
T RNG<T>::Next() {
  if (m_Queue.IsEmpty()) {
    throw std::runtime_error("RNG queue is empty. Call Fill() before Next().");
  }
  return m_Queue.Pop();
}

template <typename T>
void RNG<T>::SetRange(T min, T max) {
  m_Range = {min, max};
  m_Distribution = Distribution(min, max);
}

template <typename T>
void RNG<T>::SetRange(const Range &range) {
  SetRange(range.min, range.max);
}

template <typename T>
typename RNG<T>::Range RNG<T>::GetRange() const {
  return m_Range;
}

template <typename T>
unsigned int RNG<T>::QueueSize() const { return m_Queue.Size(); }

template <typename T>
bool RNG<T>::IsQueueEmpty() const { return m_Queue.IsEmpty(); }

template <typename T>
void RNG<T>::ClearQueue() { m_Queue.Clear(); }

} // namespace GLib::Util
