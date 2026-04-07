#include "g-lib/ds/SQueue.h"
#include <chrono>
#include <iostream>
#include <queue>

using namespace GLib::DS;

static const int N = 100000;

int main() {
  std::cout << "=== SQueue vs std::queue (" << N << " entries) ===" << std::endl;

  // --- GLib SQueue ---
  {
    SQueue<int> bench;

    auto t0 = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < N; ++i) bench.Push(i);
    auto t1 = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < N; ++i) bench.Peek();
    auto t2 = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < N; ++i) bench.Pop();
    auto t3 = std::chrono::high_resolution_clock::now();

    std::cout << "  GLib SQueue  — "
              << "Push: " << std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0).count() << "us  "
              << "Peek: " << std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count() << "us  "
              << "Pop: "  << std::chrono::duration_cast<std::chrono::microseconds>(t3 - t2).count() << "us"
              << std::endl;
  }

  // --- std::queue ---
  {
    std::queue<int> bench;

    auto t0 = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < N; ++i) bench.push(i);
    auto t1 = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < N; ++i) { volatile int v = bench.front(); (void)v; }
    auto t2 = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < N; ++i) bench.pop();
    auto t3 = std::chrono::high_resolution_clock::now();

    std::cout << "  std::queue   — "
              << "Push: " << std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0).count() << "us  "
              << "Peek: " << std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count() << "us  "
              << "Pop: "  << std::chrono::duration_cast<std::chrono::microseconds>(t3 - t2).count() << "us"
              << std::endl;
  }

  return 0;
}
