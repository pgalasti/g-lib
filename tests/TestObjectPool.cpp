#include "g-lib/core/ObjectPool.h"
#include <cassert>
#include <iostream>
#include <string>

using namespace GLib::Core;

struct Vec3 {
  float x, y, z;
  Vec3(float x, float y, float z) : x(x), y(y), z(z) {}
};

static void TestAcquireRelease() {
  std::cout << "ObjectPool Test :: Acquire returns usable memory, Release returns it..." << std::endl;

  ObjectPool<Vec3> pool(8);

  Vec3* p = pool.Acquire();
  ::new (p) Vec3(1.0f, 2.0f, 3.0f);
  assert(p->x == 1.0f && p->y == 2.0f && p->z == 3.0f);
  p->~Vec3();
  pool.Release(p);

  assert(pool.Available() == 8);
  assert(pool.InUse()     == 0);
}

static void TestNewDelete() {
  std::cout << "ObjectPool Test :: New constructs and Delete destructs..." << std::endl;

  struct Tracked {
    int   value;
    bool& destroyed;
    Tracked(int v, bool& flag) : value(v), destroyed(flag) {}
    ~Tracked() { destroyed = true; }
  };

  ObjectPool<Tracked> pool(4);
  bool gone = false;

  Tracked* obj = pool.New(42, gone);
  assert(obj->value == 42);
  assert(!gone);
  assert(pool.InUse() == 1);

  pool.Delete(obj);
  assert(gone);
  assert(pool.InUse() == 0);
}

static void TestDiagnostics() {
  std::cout << "ObjectPool Test :: Available and InUse track slot state..." << std::endl;

  ObjectPool<int> pool(4);

  assert(pool.Capacity()  == 4);
  assert(pool.Available() == 4);
  assert(pool.InUse()     == 0);

  int* a = pool.New(1);
  int* b = pool.New(2);
  assert(pool.Available() == 2);
  assert(pool.InUse()     == 2);

  pool.Delete(a);
  assert(pool.Available() == 3);
  assert(pool.InUse()     == 1);

  pool.Delete(b);
  assert(pool.Available() == 4);
  assert(pool.InUse()     == 0);
}

static void TestOwns() {
  std::cout << "ObjectPool Test :: Owns identifies pointers from this pool..." << std::endl;

  ObjectPool<int> pool(4);

  int* p = pool.Acquire();
  assert(pool.Owns(p));

  int external = 0;
  assert(!pool.Owns(&external));

  pool.Release(p);
}

static void TestExhaustionThrows() {
  std::cout << "ObjectPool Test :: std::bad_alloc thrown when pool exhausted..." << std::endl;

  ObjectPool<int> pool(2);
  int* a = pool.Acquire();
  int* b = pool.Acquire();

  bool threw = false;
  try {
    pool.Acquire();
  } catch (const std::bad_alloc&) {
    threw = true;
  }
  assert(threw);

  pool.Release(a);
  pool.Release(b);
}

static void TestSlotsAreReused() {
  std::cout << "ObjectPool Test :: Released slots are reused across cycles..." << std::endl;

  ObjectPool<int> pool(2);

  for (int round = 0; round < 10; ++round) {
    int* a = pool.New(round);
    int* b = pool.New(round + 1);
    assert(*a == round);
    assert(*b == round + 1);
    pool.Delete(a);
    pool.Delete(b);
  }

  assert(pool.Available() == 2);
}

int main() {
  TestAcquireRelease();
  TestNewDelete();
  TestDiagnostics();
  TestOwns();
  TestExhaustionThrows();
  TestSlotsAreReused();

  std::cout << "ObjectPool tests passed!" << std::endl;
  return 0;
}
