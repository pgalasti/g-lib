#ifndef G_LIB_LEAK_DETECTOR_H
#define G_LIB_LEAK_DETECTOR_H

#include <atomic>

namespace GLib::Memory {

// Safe to include from any translation unit.
// Usage() returns 0 when leak detection is not activated.
class LeakDetector {

  // Signed so that a mismatched free shows as negative rather than wrapping.
  std::atomic<long long> inUse;

  LeakDetector() : inUse{0LL} {}

public:
  LeakDetector(const LeakDetector&)            = delete;
  LeakDetector& operator=(const LeakDetector&) = delete;

  static LeakDetector& instance() {
    static LeakDetector ld;
    return ld;
  }

  enum class AllocType {
    Credit, Debit
  };

  void Record(AllocType type, std::size_t memory) noexcept {
    if (type == AllocType::Credit) {
      inUse += static_cast<long long>(memory);
    } else {
      inUse -= static_cast<long long>(memory);
    }
  }

  // Returns signed bytes in use. Negative means more frees than allocs (a bug).
  long long Usage() const noexcept {
    return inUse.load();
  }
};

} // namespace GLib::Memory

#endif // G_LIB_LEAK_DETECTOR_H
