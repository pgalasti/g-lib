#define ENABLE_LEAK_DETECTOR
#include "g-lib/memory/LeakDetectorActivate.h"
#include "g-lib/memory/LeakDetector.h"
#include <cassert>
#include <iostream>

using namespace GLib::Memory;

struct Point { int x, y; };

// Helper: prevents GCC from applying new-expression allocation elision to
// array tests. new int[N]{} (no initializer) is recognized as a dead allocation
// when the pointer only lives long enough to be deleted, so use explicit operator
// calls which GCC treats as opaque external functions.
template<typename T>
T* AllocArray(std::size_t n) {
  return static_cast<T*>(::operator new[](n * sizeof(T)));
}
template<typename T>
void FreeArray(T* ptr, std::size_t n) {
  ::operator delete[](static_cast<void*>(ptr), n * sizeof(T));
}

int main() {
  std::cout << "Testing LeakDetector..." << std::endl;

  auto& ld = LeakDetector::instance();

  // --- Scalar new/delete is tracked ---
  // Initializers (e.g. 42, 3.14) write to the allocated memory, which gives the
  // pointer a live use and prevents GCC's dead-allocation-expression elision.
  std::cout << "LeakDetector Test :: new int increases usage by sizeof(int)..." << std::endl;
  {
    auto before = ld.Usage();
    int* p = new int(42);
    assert(ld.Usage() == before + static_cast<long long>(sizeof(int)));
    delete p;
    assert(ld.Usage() == before);
  }

  std::cout << "LeakDetector Test :: new double increases usage by sizeof(double)..." << std::endl;
  {
    auto before = ld.Usage();
    double* p = new double(3.14);
    assert(ld.Usage() == before + static_cast<long long>(sizeof(double)));
    delete p;
    assert(ld.Usage() == before);
  }

  std::cout << "LeakDetector Test :: new Point increases usage by sizeof(Point)..." << std::endl;
  {
    auto before = ld.Usage();
    Point* p = new Point{3, 4};
    assert(ld.Usage() == before + static_cast<long long>(sizeof(Point)));
    delete p;
    assert(ld.Usage() == before);
  }

  // --- Multiple scalar allocations accumulate correctly ---
  std::cout << "LeakDetector Test :: multiple scalar allocations accumulate correctly..." << std::endl;
  {
    auto before = ld.Usage();
    int* a = new int(1);
    int* b = new int(2);
    int* c = new int(3);
    assert(ld.Usage() == before + 3LL * static_cast<long long>(sizeof(int)));
    delete a;
    delete b;
    delete c;
    assert(ld.Usage() == before);
  }

  // --- Array new[]/delete[] is tracked ---
  // Use explicit ::operator new[]/ ::operator delete[] calls to test the array
  // overrides directly. GCC elides new-expression pairs whose pointer has no
  // initializer and never escapes, even across noinline/noipa boundaries.
  std::cout << "LeakDetector Test :: operator new[] increases usage by N*sizeof(int)..." << std::endl;
  {
    constexpr std::size_t N = 5;
    auto before = ld.Usage();
    int* arr = AllocArray<int>(N);
    assert(ld.Usage() == before + static_cast<long long>(N * sizeof(int)));
    FreeArray(arr, N);
    assert(ld.Usage() == before);
  }

  std::cout << "LeakDetector Test :: operator new[] increases usage by N*sizeof(double)..." << std::endl;
  {
    constexpr std::size_t N = 3;
    auto before = ld.Usage();
    double* arr = AllocArray<double>(N);
    assert(ld.Usage() == before + static_cast<long long>(N * sizeof(double)));
    FreeArray(arr, N);
    assert(ld.Usage() == before);
  }

  // --- Mixed scalar and array allocations ---
  std::cout << "LeakDetector Test :: mixed scalar and array allocations tracked correctly..." << std::endl;
  {
    auto before = ld.Usage();
    int*    p   = new int(10);
    double* arr = AllocArray<double>(3);
    auto expected = before
      + static_cast<long long>(sizeof(int))
      + static_cast<long long>(3 * sizeof(double));
    assert(ld.Usage() == expected);
    delete p;
    FreeArray(arr, 3);
    assert(ld.Usage() == before);
  }

  // --- Interleaved alloc/dealloc stays balanced ---
  std::cout << "LeakDetector Test :: interleaved alloc/dealloc stays balanced..." << std::endl;
  {
    auto before = ld.Usage();
    int* p = new int(1);
    delete p;
    int* q = new int(2);
    delete q;
    assert(ld.Usage() == before);
  }

  // --- Unfreed allocation is detectable ---
  std::cout << "LeakDetector Test :: unfreed allocation shows as positive usage delta..." << std::endl;
  {
    auto before = ld.Usage();
    int* leaked = new int(99);
    assert(ld.Usage() > before);
    delete leaked; // clean up
  }

  // --- delete nullptr is a no-op ---
  std::cout << "LeakDetector Test :: delete nullptr does not affect usage..." << std::endl;
  {
    auto before = ld.Usage();
    int* p = nullptr;
    delete p;
    assert(ld.Usage() == before);
  }

  std::cout << "LeakDetector tests passed!" << std::endl;
  return 0;
}
