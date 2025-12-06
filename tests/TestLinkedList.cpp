#include "g-lib/ds/LinkedList.h"
#include <cassert>
#include <iostream>

using namespace GLib::DS;

int main() {
  std::cout << "Testing LinkedList..." << std::endl;

  // Test 1: Initialization
  LinkedList<int> list;
  assert(list.size() == 0);

  // Test 2: Push Back
  list.Push(10, LinkedListEnd::END);
  assert(list.size() == 1);
  assert(list.Peek(LinkedListEnd::END) == 10);
  assert(list.Peek(LinkedListEnd::BEGINNING) == 10);

  list.Push(20, LinkedListEnd::END);
  assert(list.size() == 2);
  assert(list.Peek(LinkedListEnd::END) == 20);
  assert(list.Peek(LinkedListEnd::BEGINNING) == 10);

  // Test 3: Push Front
  list.Push(5, LinkedListEnd::BEGINNING);
  assert(list.size() == 3);
  assert(list.Peek(LinkedListEnd::BEGINNING) == 5);
  assert(list.Peek(LinkedListEnd::END) == 20);

  // Current list: [5, 10, 20]

  // Test 4: Pop Front
  int val = list.Pop(LinkedListEnd::BEGINNING);
  assert(val == 5);
  assert(list.size() == 2);
  assert(list.Peek(LinkedListEnd::BEGINNING) == 10);

  // Current list: [10, 20]

  // Test 5: Pop Back
  val = list.Pop(LinkedListEnd::END);
  assert(val == 20);
  assert(list.size() == 1);
  assert(list.Peek(LinkedListEnd::END) == 10);

  // Current list: [10]

  // Test 6: Clear
  list.Clear();
  assert(list.size() == 0);

  // Test 7: Pop from empty (should handle gracefully, maybe return default T)
  val = list.Pop();
  assert(val == 0); // Assuming default int is 0

  std::cout << "LinkedList tests passed!" << std::endl;
  return 0;
}
