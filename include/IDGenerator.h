#ifndef ID_GENERATOR_H
#define ID_GENERATOR_H

#include <memory>
#include <queue>

namespace GLib {

template <typename T> class IDGenerator {
public:
  IDGenerator(const T firstValue, const T incrementAmount = 100);

  T GetNextId();

  void Reset(const T firstValue);

  void Release(const T id);

  T GetLatestId() { return m_LastId; }
  T GetFirstValue() { return m_FirstValue; }

private:
  T m_LastId;
  T m_FirstValue;
  T m_IncrementValue;

  std::queue<T> m_IdQueue;
};
} // namespace GLib

#endif
