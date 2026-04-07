#include "g-lib/ds/LinkedList.h"
#include <chrono>
#include <iostream>
#include <list>

using namespace GLib::DS;

static const int N = 100000;

int main() {
  std::cout << "=== LinkedList vs std::list (" << N << " entries) ===" << std::endl;

  // --- GLib LinkedList ---
  {
    LinkedList<int> bench;

    auto t0 = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < N; ++i) bench.Push(i);
    auto t1 = std::chrono::high_resolution_clock::now();
    bench.Contains(N / 2);
    auto t2 = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < N; ++i) bench.Pop();
    auto t3 = std::chrono::high_resolution_clock::now();

    std::cout << "  GLib LinkedList — "
              << "Push: "    << std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0).count() << "us  "
              << "Contains: " << std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count() << "us  "
              << "Pop: "     << std::chrono::duration_cast<std::chrono::microseconds>(t3 - t2).count() << "us"
              << std::endl;
  }

  // --- std::list ---
  {
    std::list<int> bench;

    auto t0 = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < N; ++i) bench.push_back(i);
    auto t1 = std::chrono::high_resolution_clock::now();
    for (auto it = bench.begin(); it != bench.end(); ++it) { if (*it == N / 2) break; }
    auto t2 = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < N; ++i) bench.pop_back();
    auto t3 = std::chrono::high_resolution_clock::now();

    std::cout << "  std::list       — "
              << "Push: "    << std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0).count() << "us  "
              << "Contains: " << std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count() << "us  "
              << "Pop: "     << std::chrono::duration_cast<std::chrono::microseconds>(t3 - t2).count() << "us"
              << std::endl;
  }

  return 0;
}
