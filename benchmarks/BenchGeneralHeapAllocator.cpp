#include "g-lib/core/GeneralHeapAllocator.h"
#include <chrono>
#include <iostream>
#include <vector>

using namespace GLib::Core;

static const int N = 1'000'000;

int main() {
  std::cout << "=== GeneralHeapAllocator vs std::allocator (vector push_back, " << N << " ints) ===" << std::endl;

  // --- Pool allocator, no reserve ---
  {
    GeneralHeapAllocator<int> gha(MemoryUnit::MB, 16);
    std::vector<int, GeneralHeapAllocator<int>> v(gha);

    auto t0 = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < N; ++i) v.push_back(i);
    auto t1 = std::chrono::high_resolution_clock::now();

    volatile int sink = v.back(); (void)sink;

    std::cout << "  Pool allocator (no reserve)  — "
              << std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0).count() << "us" << std::endl;
  }

  // --- std::allocator, no reserve ---
  {
    std::vector<int> v;

    auto t0 = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < N; ++i) v.push_back(i);
    auto t1 = std::chrono::high_resolution_clock::now();

    volatile int sink = v.back(); (void)sink;

    std::cout << "  std::allocator (no reserve)  — "
              << std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0).count() << "us" << std::endl;
  }

  // --- Pool allocator, with reserve ---
  {
    GeneralHeapAllocator<int> gha(MemoryUnit::MB, 16);
    std::vector<int, GeneralHeapAllocator<int>> v(gha);
    v.reserve(N);

    auto t0 = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < N; ++i) v.push_back(i);
    auto t1 = std::chrono::high_resolution_clock::now();

    volatile int sink = v.back(); (void)sink;

    std::cout << "  Pool allocator (reserve)     — "
              << std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0).count() << "us" << std::endl;
  }

  // --- std::allocator, with reserve ---
  {
    std::vector<int> v;
    v.reserve(N);

    auto t0 = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < N; ++i) v.push_back(i);
    auto t1 = std::chrono::high_resolution_clock::now();

    volatile int sink = v.back(); (void)sink;

    std::cout << "  std::allocator (reserve)     — "
              << std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0).count() << "us" << std::endl;
  }

  return 0;
}
