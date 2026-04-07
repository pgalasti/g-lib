#include "g-lib/ds/HashMap.h"
#include <chrono>
#include <iostream>
#include <unordered_map>

using namespace GLib::DS;

static const int N = 100000;

int main() {
  std::cout << "=== HashMap vs std::unordered_map (" << N << " entries) ===" << std::endl;

  // --- GLib Hashmap ---
  {
    Hashmap<int, int> bench(N);

    auto t0 = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < N; ++i) bench.Insert(i, i * 2);
    auto t1 = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < N; ++i) bench.Contains(i);
    auto t2 = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < N; ++i) bench.Get(i);
    auto t3 = std::chrono::high_resolution_clock::now();

    std::cout << "  GLib Hashmap       — "
              << "Insert: "   << std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0).count() << "us  "
              << "Contains: " << std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count() << "us  "
              << "Get: "      << std::chrono::duration_cast<std::chrono::microseconds>(t3 - t2).count() << "us"
              << std::endl;
  }

  // --- std::unordered_map ---
  {
    std::unordered_map<int, int> bench;
    bench.reserve(N);

    auto t0 = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < N; ++i) bench.emplace(i, i * 2);
    auto t1 = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < N; ++i) bench.count(i);
    auto t2 = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < N; ++i) bench.at(i);
    auto t3 = std::chrono::high_resolution_clock::now();

    std::cout << "  std::unordered_map — "
              << "Insert: "   << std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0).count() << "us  "
              << "Contains: " << std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count() << "us  "
              << "Get: "      << std::chrono::duration_cast<std::chrono::microseconds>(t3 - t2).count() << "us"
              << std::endl;
  }

  return 0;
}
