#include "g-lib/ds/Stack.h"
#include <chrono>
#include <iostream>
#include <stack>

using namespace GLib::DS;

static const int N = 100000;

int main() {
  std::cout << "=== Stack vs std::stack (" << N << " entries) ===" << std::endl;

  // --- GLib Stack ---
  {
    Stack<int> bench;

    auto t0 = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < N; ++i) bench.Push(i);
    auto t1 = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < N; ++i) bench.Peek();
    auto t2 = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < N; ++i) bench.Pop();
    auto t3 = std::chrono::high_resolution_clock::now();

    std::cout << "  GLib Stack  — "
              << "Push: " << std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0).count() << "us  "
              << "Peek: " << std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count() << "us  "
              << "Pop: "  << std::chrono::duration_cast<std::chrono::microseconds>(t3 - t2).count() << "us"
              << std::endl;
  }

  // --- std::stack ---
  {
    std::stack<int> bench;

    auto t0 = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < N; ++i) bench.push(i);
    auto t1 = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < N; ++i) { volatile int v = bench.top(); (void)v; }
    auto t2 = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < N; ++i) bench.pop();
    auto t3 = std::chrono::high_resolution_clock::now();

    std::cout << "  std::stack  — "
              << "Push: " << std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0).count() << "us  "
              << "Peek: " << std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count() << "us  "
              << "Pop: "  << std::chrono::duration_cast<std::chrono::microseconds>(t3 - t2).count() << "us"
              << std::endl;
  }

  return 0;
}
