#include "g-lib/ds/LinkedList.h"
#include "g-lib/ds/Seek.h"
#include <cassert>
#include <iostream>

using namespace GLib::DS;

int main() {
  std::cout << "Testing LinkedList..." << std::endl;

  // Test 1: Initialization
  std::cout << "LinkedList :: Testing Initialization" << std::endl;
  LinkedList<int> list;
  assert(list.Size() == 0);

  // Test 2: Push Back
  std::cout << "LinkedList :: Testing Push on end" << std::endl;
  list.Push(10, LinkedListEnd::END);
  assert(list.Size() == 1);
  assert(list.Peek(LinkedListEnd::END) == 10);
  assert(list.Peek(LinkedListEnd::BEGINNING) == 10);

  list.Push(20, LinkedListEnd::END);
  assert(list.Size() == 2);
  assert(list.Peek(LinkedListEnd::END) == 20);
  assert(list.Peek(LinkedListEnd::BEGINNING) == 10);

  // Test 3: Push Front
  std::cout << "LinkedList :: Testing Push at front" << std::endl;
  list.Push(5, LinkedListEnd::BEGINNING);
  assert(list.Size() == 3);
  assert(list.Peek(LinkedListEnd::BEGINNING) == 5);
  assert(list.Peek(LinkedListEnd::END) == 20);

  // Current list: [5, 10, 20]

  // Test 4: Pop Front
  std::cout << "LinkedList :: Testing Pop Front" << std::endl;
  int val = list.Pop(LinkedListEnd::BEGINNING);
  assert(val == 5);
  assert(list.Size() == 2);
  assert(list.Peek(LinkedListEnd::BEGINNING) == 10);

  // Current list: [10, 20]

  // Test 5: Pop Back
  std::cout << "LinkedList :: Testing Pop Back" << std::endl;
  val = list.Pop(LinkedListEnd::END);
  assert(val == 20);
  assert(list.Size() == 1);
  assert(list.Peek(LinkedListEnd::END) == 10);

  // Current list: [10]

  // Test 6: Clear
  std::cout << "LinkedList :: Testing Clear" << std::endl;
  list.Clear();
  assert(list.Size() == 0);

  // Test 7: Pop from empty (should handle gracefull)
  std::cout << "LinkedList :: Testing Pop on empty" << std::endl;
  try {
    val = list.Pop();
    assert(val == 0); // Assuming default int is 0
  } catch (const std::runtime_error &e) {
    std::cout << "Expected error for pop on empty" << std::endl;
  }

  // Test 8: String Type
  std::cout << "LinkedList :: Testing std::string type" << std::endl;
  LinkedList<std::string> strList;
  strList.Push("Hello", LinkedListEnd::END);
  strList.Push("World", LinkedListEnd::END);
  assert(strList.Size() == 2);
  assert(strList.Pop(LinkedListEnd::BEGINNING) == "Hello");
  assert(strList.Pop(LinkedListEnd::BEGINNING) == "World");

  // Test 9: Test Seek Interface
  LinkedList<int> seekTestList;
  seekTestList.Push(1);
  seekTestList.Push(2);
  seekTestList.Push(3);
  seekTestList.Push(4);
  seekTestList.Push(5);
  seekTestList.Push(6);

  Seek<int>& seek = seekTestList;
  assert(seek.Contains(3));
  assert(!seek.Contains(99));

  std::cout << "LinkedList tests passed!" << std::endl;
  return 0;
}
