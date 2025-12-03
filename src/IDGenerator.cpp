#include "IDGenerator.h"

namespace GLib {

template <typename T>
IDGenerator<T>::IDGenerator(const T firstValue, const T incrementAmount) {
  this->Reset(firstValue);
  this->m_IncrementValue = incrementAmount;
}

template <typename T> T IDGenerator<T>::GetNextId() {

  if (m_IdQueue.empty()) {
    for (T i = m_LastId + 1; i < m_LastId + m_IncrementValue; i++)
      m_IdQueue.push(i);
  }

  // Get the next ID from the queue and pop it.
  m_LastId = m_IdQueue.front();
  m_IdQueue.pop();

  return m_LastId;
}

template <typename T> void IDGenerator<T>::Reset(const T firstValue) {
  // Clear out any remaining values.
  while (!m_IdQueue.empty())
    m_IdQueue.pop();

  // Give the 100 IDs to start with
  for (T i = firstValue; i < firstValue + 100; i++)
    m_IdQueue.push(i);

  m_FirstValue = m_LastId = firstValue;
}

template <typename T> void IDGenerator<T>::Release(const T id) {
  if (id > m_LastId)
    return; // Can't release something we never had assigned.

  m_IdQueue.push(id);
}

// Explicit instantiation
template class IDGenerator<int>;

} // namespace GLib
