#include "g-lib/ds/DQueue.h"
#include <chrono>
#include <deque>
#include <iostream>

using namespace GLib::DS;

static const int N = 100000;

int main() {
  std::cout << "=== DQueue vs std::deque (" << N << " entries) ===" << std::endl;

  // --- GLib DQueue ---
  {
    DQueue<int> bench;

    auto t0 = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < N; ++i) bench.Push(i);
    auto t1 = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < N / 2; ++i) bench.PushFront(i);
    auto t2 = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < N / 2; ++i) bench.Pop();
    auto t3 = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < N; ++i) bench.PopBack();
    auto t4 = std::chrono::high_resolution_clock::now();

    std::cout << "  GLib DQueue  — "
              << "Push: "      << std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0).count() << "us  "
              << "PushFront: " << std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count() << "us  "
              << "Pop: "       << std::chrono::duration_cast<std::chrono::microseconds>(t3 - t2).count() << "us  "
              << "PopBack: "   << std::chrono::duration_cast<std::chrono::microseconds>(t4 - t3).count() << "us"
              << std::endl;
  }

  // --- std::deque ---
  {
    std::deque<int> bench;

    auto t0 = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < N; ++i) bench.push_back(i);
    auto t1 = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < N / 2; ++i) bench.push_front(i);
    auto t2 = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < N / 2; ++i) bench.pop_front();
    auto t3 = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < N; ++i) bench.pop_back();
    auto t4 = std::chrono::high_resolution_clock::now();

    std::cout << "  std::deque   — "
              << "Push: "      << std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0).count() << "us  "
              << "PushFront: " << std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count() << "us  "
              << "Pop: "       << std::chrono::duration_cast<std::chrono::microseconds>(t3 - t2).count() << "us  "
              << "PopBack: "   << std::chrono::duration_cast<std::chrono::microseconds>(t4 - t3).count() << "us"
              << std::endl;
  }

  return 0;
}
