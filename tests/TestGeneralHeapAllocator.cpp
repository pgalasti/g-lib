#include "g-lib/core/GeneralHeapAllocator.h"
#include <cassert>
#include <iostream>
#include <string>
#include <vector>

using namespace GLib::Core;

// ── GeneralHeapAllocator (STL interface) ─────────────────────────────────────

static void TestBasicAllocateDeallocate() {
  std::cout << "GeneralHeapAllocator Test :: Basic allocate / deallocate..." << std::endl;

  GeneralHeapAllocator<int> gha(MemoryUnit::KB, 64);

  int* p = gha.allocate(1);
  assert(p != nullptr);
  *p = 42;
  assert(*p == 42);
  gha.deallocate(p, 1);
}

static void TestMultipleAllocationsPreserveValues() {
  std::cout << "GeneralHeapAllocator Test :: Multiple allocations preserve values..." << std::endl;

  GeneralHeapAllocator<int> gha(MemoryUnit::KB, 64);

  constexpr int N = 100;
  int* ptrs[N];

  for (int i = 0; i < N; ++i) {
    ptrs[i] = gha.allocate(1);
    *ptrs[i] = i;
  }

  for (int i = 0; i < N; ++i)
    assert(*ptrs[i] == i);

  for (int i = 0; i < N; ++i)
    gha.deallocate(ptrs[i], 1);
}

static void TestPoolReuseAfterDeallocate() {
  std::cout << "GeneralHeapAllocator Test :: Pool memory reused after deallocation..." << std::endl;

  GeneralHeapAllocator<int> gha(MemoryUnit::KB, 4);

  for (int round = 0; round < 5; ++round) {
    int* p = gha.allocate(256); // 256 ints = 1 KB
    p[0] = round;
    assert(p[0] == round);
    gha.deallocate(p, 256);
  }
}

static void TestVectorWithAllocator() {
  std::cout << "GeneralHeapAllocator Test :: std::vector with pool allocator, 1000000 ints..." << std::endl;

  GeneralHeapAllocator<int> gha(MemoryUnit::MB, 16);
  std::vector<int, GeneralHeapAllocator<int>> v(gha);

  constexpr int N = 1'000'000;
  v.reserve(N);

  for (int i = 0; i < N; ++i)
    v.push_back(i);

  assert(static_cast<int>(v.size()) == N);
  for (int i = 0; i < N; ++i)
    assert(v[i] == i);
}

static void TestSharedPoolAcrossCopies() {
  std::cout << "GeneralHeapAllocator Test :: Copied allocators share pool..." << std::endl;

  GeneralHeapAllocator<int> a(MemoryUnit::KB, 64);
  GeneralHeapAllocator<int> b = a;

  int* pa = a.allocate(1); *pa = 1;
  int* pb = b.allocate(1); *pb = 2;

  assert(*pa == 1);
  assert(*pb == 2);
  assert(a == b);

  a.deallocate(pa, 1);
  b.deallocate(pb, 1);
}

static void TestBadAllocOnExhaustion() {
  std::cout << "GeneralHeapAllocator Test :: std::bad_alloc thrown when pool exhausted..." << std::endl;

  GeneralHeapAllocator<int> gha(MemoryUnit::KB, 1);

  bool threw = false;
  try {
    gha.allocate(1'000'000);
  } catch (const std::bad_alloc&) {
    threw = true;
  }

  assert(threw);
}

// ── MemoryPool (direct interface) ────────────────────────────────────────────

static void TestAcquireRelease() {
  std::cout << "MemoryPool Test :: Acquire and Release raw typed memory..." << std::endl;

  MemoryPool pool(MemoryUnit::KB, 64);

  float* arr = pool.Acquire<float>(4);
  arr[0] = 1.0f; arr[1] = 2.0f; arr[2] = 3.0f; arr[3] = 4.0f;

  assert(arr[0] == 1.0f);
  assert(arr[3] == 4.0f);

  pool.Release(arr, 4);
}

static void TestNewDelete() {
  std::cout << "MemoryPool Test :: New constructs and Delete destructs objects..." << std::endl;

  struct Tracked {
    int   value;
    bool& destroyed;
    Tracked(int v, bool& flag) : value(v), destroyed(flag) {}
    ~Tracked() { destroyed = true; }
  };

  MemoryPool pool(MemoryUnit::KB, 64);
  bool gone = false;

  Tracked* obj = pool.New<Tracked>(99, gone);
  assert(obj->value == 99);
  assert(!gone);

  pool.Delete(obj);
  assert(gone);
}

static void TestDiagnostics() {
  std::cout << "MemoryPool Test :: BytesUsed and BytesFree track allocations..." << std::endl;

  MemoryPool pool(MemoryUnit::KB, 64);

  assert(pool.BytesUsed() == 0);
  assert(pool.BytesFree() == pool.Capacity());

  int* p = pool.Acquire<int>(1);
  assert(pool.BytesUsed() > 0);
  assert(pool.BytesFree() < pool.Capacity());

  pool.Release(p, 1);
  assert(pool.BytesUsed() == 0);
  assert(pool.BytesFree() == pool.Capacity());
}

static void TestReset() {
  std::cout << "MemoryPool Test :: Reset restores full capacity in O(1)..." << std::endl;

  MemoryPool pool(MemoryUnit::KB, 64);

  // Allocate several things and then bulk-discard without individual releases.
  pool.Acquire<int>(10);
  pool.Acquire<double>(5);
  pool.Acquire<char>(32);

  assert(pool.BytesUsed() > 0);

  pool.Reset();

  assert(pool.BytesUsed() == 0);
  assert(pool.BytesFree() == pool.Capacity());

  // Pool should be fully usable again after reset.
  int* p = pool.Acquire<int>(1);
  *p = 7;
  assert(*p == 7);
  pool.Release(p, 1);
}

static void TestSharedPoolBetweenAllocatorAndDirectUse() {
  std::cout << "MemoryPool Test :: Pool shared between GHA and direct Acquire/New..." << std::endl;

  auto pool = std::make_shared<MemoryPool>(MemoryUnit::MB, 4);

  // Give the same pool to a vector allocator.
  GeneralHeapAllocator<int> gha(pool);
  std::vector<int, GeneralHeapAllocator<int>> v(gha);
  v.reserve(100);
  for (int i = 0; i < 100; ++i) v.push_back(i);

  // Also allocate directly from the same pool.
  float* scratch = pool->Acquire<float>(16);
  scratch[0] = 3.14f;

  assert(v[99] == 99);
  assert(scratch[0] == 3.14f);

  // Both drew from the same pool — BytesUsed reflects both.
  assert(pool->BytesUsed() > 0);

  pool->Release(scratch, 16);
}

int main() {
  TestBasicAllocateDeallocate();
  TestMultipleAllocationsPreserveValues();
  TestPoolReuseAfterDeallocate();
  TestVectorWithAllocator();
  TestSharedPoolAcrossCopies();
  TestBadAllocOnExhaustion();

  TestAcquireRelease();
  TestNewDelete();
  TestDiagnostics();
  TestReset();
  TestSharedPoolBetweenAllocatorAndDirectUse();

  std::cout << "GeneralHeapAllocator tests passed!" << std::endl;
  return 0;
}
