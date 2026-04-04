#ifndef GLIB_RNG_H
#define GLIB_RNG_H

#include <random>
#include <type_traits>
#include "g-lib/ds/SQueue.h"

namespace GLib::Util {

template <typename T = int>
class RNG {
public:
  struct Range {
    T min;
    T max;
  };

  RNG(T min, T max);
  RNG(const Range &range);

  // Doesn't use a fill queue
  T Generate();

  void Fill(unsigned int count);

  T Next();

  void SetRange(T min, T max);
  void SetRange(const Range &range);
  Range GetRange() const;

  unsigned int QueueSize() const;
  bool IsQueueEmpty() const;
  void ClearQueue();

private:
  using Distribution = std::conditional_t<
      std::is_integral_v<T>,
      std::uniform_int_distribution<T>,
      std::uniform_real_distribution<T>>;

  Range m_Range;
  std::mt19937 m_Engine;
  Distribution m_Distribution;
  GLib::DS::SQueue<T> m_Queue;
};

} // namespace GLib::Util

#include "RNG.tpp"
#endif
