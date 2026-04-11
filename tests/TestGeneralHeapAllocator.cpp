#include "g-lib/core/GeneralHeapAllocator.h"
#include <cassert>
#include <iostream>
#include <vector>

using namespace GLib::Core;

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

int main() {
  TestBasicAllocateDeallocate();
  TestMultipleAllocationsPreserveValues();
  TestPoolReuseAfterDeallocate();
  TestVectorWithAllocator();
  TestSharedPoolAcrossCopies();
  TestBadAllocOnExhaustion();

  std::cout << "GeneralHeapAllocator tests passed!" << std::endl;
  return 0;
}
