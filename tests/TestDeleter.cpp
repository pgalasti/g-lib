#include "g-lib/core/Deleter.h"
#include <cassert>
#include <chrono>
#include <iostream>
#include <thread>

using namespace std::chrono_literals;

// Tracks whether its destructor was called via an external flag.
struct Tracked {
  bool& deleted;
  explicit Tracked(bool& flag) : deleted(flag) {}
  ~Tracked() { deleted = true; }
};

int main() {
  std::cout << "Testing Deleter..." << std::endl;

  // --- Flush ---
  std::cout << "Deleter Test :: Flush immediately deletes queued items..." << std::endl;
  {
    GLib::Core::Deleter deleter(1000ms);
    bool gone = false;
    deleter.Schedule(new Tracked(gone));
    assert(!gone); // not deleted yet
    deleter.Flush();
    assert(gone);  // deleted after explicit flush
  }

  // --- Interval-based deletion ---
  std::cout << "Deleter Test :: Scheduled items deleted after interval..." << std::endl;
  {
    GLib::Core::Deleter deleter(50ms);
    bool gone = false;
    deleter.Schedule(new Tracked(gone));
    assert(!gone);
    std::this_thread::sleep_for(150ms); // wait for at least one cycle
    assert(gone);
  }

  // --- Multiple items ---
  std::cout << "Deleter Test :: Multiple scheduled items all deleted..." << std::endl;
  {
    GLib::Core::Deleter deleter(50ms);
    bool a = false, b = false, c = false;
    deleter.Schedule(new Tracked(a));
    deleter.Schedule(new Tracked(b));
    deleter.Schedule(new Tracked(c));
    std::this_thread::sleep_for(150ms);
    assert(a && b && c);
  }

  // --- Multiple types ---
  std::cout << "Deleter Test :: Works with different pointer types..." << std::endl;
  {
    GLib::Core::Deleter deleter(50ms);
    bool trackedGone = false;
    deleter.Schedule(new Tracked(trackedGone));
    deleter.Schedule(new int(42));
    deleter.Schedule(new double(3.14));
    std::this_thread::sleep_for(150ms);
    assert(trackedGone);
  }

  // --- Destructor flushes remaining items ---
  std::cout << "Deleter Test :: Destructor flushes remaining queued items..." << std::endl;
  {
    bool gone = false;
    {
      GLib::Core::Deleter deleter(1000ms); // long interval — won't fire naturally
      deleter.Schedule(new Tracked(gone));
      assert(!gone);
    } // destructor runs here
    assert(gone);
  }

  std::cout << "Deleter tests passed!" << std::endl;
  return 0;
}
