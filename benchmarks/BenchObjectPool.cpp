#include "g-lib/core/ObjectPool.h"
#include <chrono>
#include <iostream>
#include <vector>

using namespace GLib::Core;

struct GameObject {
  float x, y, z;
  int   id;
  GameObject(float x, float y, float z, int id) : x(x), y(y), z(z), id(id) {}
};

// Batch size per round — simulates acquiring a group of live objects,
// using them, then releasing the whole group (e.g. frame-based game objects,
// per-request server objects, parser AST nodes).
static const int BATCH  = 1'000;
static const int ROUNDS = 1'000;

int main() {
  std::cout << "=== ObjectPool<GameObject> vs new/delete" << std::endl;
  std::cout << "    " << ROUNDS << " rounds x " << BATCH << " objects — acquire all, use all, release all ===" << std::endl;

  std::vector<GameObject*> ptrs(BATCH);

  // --- ObjectPool (thread-safe, std::mutex) ---
  {
    ObjectPool<GameObject> pool(BATCH);

    auto t0 = std::chrono::high_resolution_clock::now();
    for (int r = 0; r < ROUNDS; ++r) {
      for (int i = 0; i < BATCH; ++i)
        ptrs[i] = pool.New(1.0f, 2.0f, 3.0f, i);

      volatile int sink = ptrs[BATCH - 1]->id; (void)sink;

      for (int i = 0; i < BATCH; ++i)
        pool.Delete(ptrs[i]);
    }
    auto t1 = std::chrono::high_resolution_clock::now();

    std::cout << "  ObjectPool (thread-safe)  — "
              << std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0).count() << "us" << std::endl;
  }

  // --- ObjectPool (single-threaded, NoMutex) ---
  {
    ObjectPool<GameObject, NoMutex> pool(BATCH);

    auto t0 = std::chrono::high_resolution_clock::now();
    for (int r = 0; r < ROUNDS; ++r) {
      for (int i = 0; i < BATCH; ++i)
        ptrs[i] = pool.New(1.0f, 2.0f, 3.0f, i);

      volatile int sink = ptrs[BATCH - 1]->id; (void)sink;

      for (int i = 0; i < BATCH; ++i)
        pool.Delete(ptrs[i]);
    }
    auto t1 = std::chrono::high_resolution_clock::now();

    std::cout << "  ObjectPool (NoMutex)      — "
              << std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0).count() << "us" << std::endl;
  }

  // --- new/delete: system allocator ---
  {
    auto t0 = std::chrono::high_resolution_clock::now();
    for (int r = 0; r < ROUNDS; ++r) {
      for (int i = 0; i < BATCH; ++i)
        ptrs[i] = new GameObject(1.0f, 2.0f, 3.0f, i);

      volatile int sink = ptrs[BATCH - 1]->id; (void)sink;

      for (int i = 0; i < BATCH; ++i)
        delete ptrs[i];
    }
    auto t1 = std::chrono::high_resolution_clock::now();

    std::cout << "  new/delete  — "
              << std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0).count() << "us" << std::endl;
  }

  return 0;
}
