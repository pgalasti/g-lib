#include "g-lib/ds/Vector.h"
#include <cassert>
#include <iostream>
#include <string>

int main() {

  // --- Initial state ---
  std::cout << "Vector Test :: Testing initial state" << std::endl;
  GLib::DS::Vector<int> v;
  assert(v.Size() == 0);
  assert(v.Capacity() == 32);

  // --- Custom initial capacity ---
  std::cout << "Vector Test :: Testing custom initial capacity" << std::endl;
  GLib::DS::Vector<int> vCustom(8);
  assert(vCustom.Size() == 0);
  assert(vCustom.Capacity() == 8);

  // --- Push and Size ---
  std::cout << "Vector Test :: Testing Push and Size" << std::endl;
  v.Push(10);
  v.Push(20);
  v.Push(30);
  assert(v.Size() == 3);
  assert(v[0] == 10);
  assert(v[1] == 20);
  assert(v[2] == 30);

  // --- operator[] mutation ---
  std::cout << "Vector Test :: Testing operator[] mutation" << std::endl;
  v[1] = 99;
  assert(v[1] == 99);

  // --- Clear ---
  std::cout << "Vector Test :: Testing Clear" << std::endl;
  v.Clear();
  assert(v.Size() == 0);
  assert(v.Capacity() == 32); // capacity unchanged after clear

  // --- Resize on Push ---
  std::cout << "Vector Test :: Testing resize on overflow" << std::endl;
  GLib::DS::Vector<int> vSmall(4);
  for (int i = 0; i < 4; ++i) vSmall.Push(i);
  assert(vSmall.Capacity() == 4);
  vSmall.Push(4); // triggers resize
  assert(vSmall.Capacity() == 8);
  assert(vSmall.Size() == 5);
  for (int i = 0; i < 5; ++i) assert(vSmall[i] == i);

  // --- Copy constructor ---
  std::cout << "Vector Test :: Testing copy constructor" << std::endl;
  GLib::DS::Vector<int> vCopy(vSmall);
  assert(vCopy.Size() == vSmall.Size());
  assert(vCopy.Capacity() == vSmall.Capacity());
  for (std::size_t i = 0; i < vSmall.Size(); ++i) assert(vCopy[i] == vSmall[i]);
  vCopy[0] = 999;
  assert(vSmall[0] == 0); // deep copy — original unaffected

  // --- Copy assignment ---
  std::cout << "Vector Test :: Testing copy assignment" << std::endl;
  GLib::DS::Vector<int> vAssign;
  vAssign = vSmall;
  assert(vAssign.Size() == vSmall.Size());
  for (std::size_t i = 0; i < vSmall.Size(); ++i) assert(vAssign[i] == vSmall[i]);

  // --- Move constructor ---
  std::cout << "Vector Test :: Testing move constructor" << std::endl;
  GLib::DS::Vector<int> vMoveSource(4);
  vMoveSource.Push(1);
  vMoveSource.Push(2);
  GLib::DS::Vector<int> vMoveDest(std::move(vMoveSource));
  assert(vMoveDest.Size() == 2);
  assert(vMoveDest[0] == 1);
  assert(vMoveDest[1] == 2);
  assert(vMoveSource.Size() == 0);     // source emptied
  assert(vMoveSource.Capacity() == 0);

  // --- Move assignment ---
  std::cout << "Vector Test :: Testing move assignment" << std::endl;
  GLib::DS::Vector<int> vMoveAssign;
  vMoveAssign = std::move(vMoveDest);
  assert(vMoveAssign.Size() == 2);
  assert(vMoveAssign[0] == 1);
  assert(vMoveDest.Size() == 0);

  // --- Works with non-trivial types ---
  std::cout << "Vector Test :: Testing with std::string" << std::endl;
  GLib::DS::Vector<std::string> vStr(4);
  vStr.Push("hello");
  vStr.Push("world");
  assert(vStr.Size() == 2);
  assert(vStr[0] == "hello");
  assert(vStr[1] == "world");
  vStr.Clear();
  assert(vStr.Size() == 0);

  std::cout << "Vector tests passed!" << std::endl;
  return 0;
}
