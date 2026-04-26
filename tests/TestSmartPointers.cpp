#include "g-lib/memory/SmartPointers.h"
#include <cassert>
#include <iostream>
#include <utility>

using namespace GLib::Memory;

struct Point { int x, y; };

struct DeleteCounter {
  static int count;
  void operator()(int* p) const { if (p) { ++count; delete p; } }
};
int DeleteCounter::count = 0;

int main() {
  std::cout << "Testing UniquePointer..." << std::endl;

  // --- Default construction ---
  std::cout << "UniquePointer Test :: Default construction yields null get()..." << std::endl;
  {
    UP<int> up;
    assert(up.get() == nullptr);
  }

  // --- Construction with raw pointer ---
  std::cout << "UniquePointer Test :: Construction with raw pointer..." << std::endl;
  {
    int* raw = new int(42);
    UP<int> up(raw);
    assert(up.get() == raw);
  }

  // --- Destructor invokes deleter ---
  std::cout << "UniquePointer Test :: Destructor invokes deleter on scope exit..." << std::endl;
  {
    DeleteCounter::count = 0;
    {
      UniquePointer<int, DeleteCounter> up(new int(1));
    }
    assert(DeleteCounter::count == 1);
  }

  // --- Default construction does not invoke deleter ---
  std::cout << "UniquePointer Test :: Default construction does not invoke deleter..." << std::endl;
  {
    DeleteCounter::count = 0;
    {
      UniquePointer<int, DeleteCounter> up;
    }
    assert(DeleteCounter::count == 0);
  }

  // --- operator* dereferences ---
  std::cout << "UniquePointer Test :: operator* returns reference to value..." << std::endl;
  {
    UP<int> up(new int(7));
    assert(*up == 7);
  }

  // --- operator* assignment ---
  std::cout << "UniquePointer Test :: operator* allows assignment through dereference..." << std::endl;
  {
    UP<int> up(new int(0));
    *up = 99;
    assert(*up == 99);
  }

  // --- operator-> member access ---
  std::cout << "UniquePointer Test :: operator-> provides member access..." << std::endl;
  {
    UP<Point> up(new Point{3, 4});
    assert(up->x == 3);
    assert(up->y == 4);
    up->x = 10;
    assert(up->x == 10);
  }

  // --- get() returns raw pointer ---
  std::cout << "UniquePointer Test :: get() returns raw pointer..." << std::endl;
  {
    int* raw = new int(5);
    UP<int> up(raw);
    assert(up.get() == raw);
    assert(*up.get() == 5);
  }

  // --- Move constructor transfers ownership ---
  std::cout << "UniquePointer Test :: Move constructor transfers ownership..." << std::endl;
  {
    int* raw = new int(10);
    UP<int> a(raw);
    UP<int> b(std::move(a));
    assert(b.get() == raw);
    assert(a.get() == nullptr);
  }

  // --- Move constructor does not double-delete ---
  std::cout << "UniquePointer Test :: Move constructor does not double-delete..." << std::endl;
  {
    DeleteCounter::count = 0;
    {
      UniquePointer<int, DeleteCounter> a(new int(1));
      {
        UniquePointer<int, DeleteCounter> b(std::move(a));
      } // b deleted here, count == 1
    }   // a destroyed here with null ptr, count unchanged
    assert(DeleteCounter::count == 1);
  }

  // --- Move assignment transfers ownership ---
  std::cout << "UniquePointer Test :: Move assignment transfers ownership..." << std::endl;
  {
    int* raw = new int(20);
    UP<int> a(raw);
    UP<int> b;
    b = std::move(a);
    assert(b.get() == raw);
    assert(a.get() == nullptr);
  }

  // --- Move assignment deletes existing resource ---
  std::cout << "UniquePointer Test :: Move assignment deletes existing resource..." << std::endl;
  {
    DeleteCounter::count = 0;
    {
      UniquePointer<int, DeleteCounter> a(new int(1));
      UniquePointer<int, DeleteCounter> b(new int(2));
      b = std::move(a); // b's old resource deleted here
      assert(DeleteCounter::count == 1);
      assert(a.get() == nullptr);
    } // b destroyed here (holds a's old resource)
    assert(DeleteCounter::count == 2);
  }

  // --- Move self-assignment is safe ---
  std::cout << "UniquePointer Test :: Move self-assignment is safe..." << std::endl;
  {
    UP<int> up(new int(42));
    int* raw = up.get();
    up = std::move(up);
    assert(up.get() == raw);
    assert(*up == 42);
  }

  // --- UP alias ---
  std::cout << "UniquePointer Test :: UP alias is equivalent..." << std::endl;
  {
    UP<int> up(new int(3));
    assert(*up == 3);
  }

  std::cout << "UniquePointer tests passed!" << std::endl;
  return 0;
}
