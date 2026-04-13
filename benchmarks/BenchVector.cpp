#include "g-lib/ds/Vector.h"
#include <chrono>
#include <iostream>
#include <vector>

using namespace GLib::DS;

static const int N = 100000;

int main() {
  std::cout << "=== Vector vs std::vector (" << N << " entries) ===" << std::endl;

  // --- GLib Vector ---
  {
    Vector<int> bench;

    auto t0 = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < N; ++i) bench.Push(i);
    auto t1 = std::chrono::high_resolution_clock::now();
    volatile int sink = 0;
    for (int i = 0; i < N; ++i) sink += bench[i];
    auto t2 = std::chrono::high_resolution_clock::now();
    bench.Clear();
    auto t3 = std::chrono::high_resolution_clock::now();

    std::cout << "  GLib Vector  — "
              << "Push: "   << std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0).count() << "us  "
              << "Access: " << std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count() << "us  "
              << "Clear: "  << std::chrono::duration_cast<std::chrono::microseconds>(t3 - t2).count() << "us"
              << std::endl;
  }

  // --- std::vector ---
  {
    std::vector<int> bench;

    auto t0 = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < N; ++i) bench.push_back(i);
    auto t1 = std::chrono::high_resolution_clock::now();
    volatile int sink = 0;
    for (int i = 0; i < N; ++i) sink += bench[i];
    auto t2 = std::chrono::high_resolution_clock::now();
    bench.clear();
    auto t3 = std::chrono::high_resolution_clock::now();

    std::cout << "  std::vector  — "
              << "Push: "   << std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0).count() << "us  "
              << "Access: " << std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count() << "us  "
              << "Clear: "  << std::chrono::duration_cast<std::chrono::microseconds>(t3 - t2).count() << "us"
              << std::endl;
  }

  return 0;
}
