#include "g-lib/ds/Stack.h"
#include <cassert>
#include <iostream>

int main() {
  std::cout << "Testing Stack..." << std::endl;

  GLib::DS::Stack<int> intStack;

  std::cout << "Testing stack initial state" << std::endl;
  assert(intStack.IsEmpty());
  assert(intStack.Size() == 0);


  std::cout << "Testing stack peek failure" << std::endl;
  try {intStack.Peek(); std::cerr << "Peek SHOULD have failed!" << std::endl;}
  catch (const std::runtime_error& e) {
    std::cout << "Peek properly failed" << std::endl;
  }

  std::cout << "Testing stack push" << std::endl;
  intStack.Push(1);
  intStack.Push(2);
  intStack.Push(3);
  intStack.Push(4);
  assert(!intStack.IsEmpty());
  assert(intStack.Size() == 4);

  std::cout << "Testing stack peek success" << std::endl;
  unsigned int shouldBeFour = intStack.Peek();
  assert(shouldBeFour == 4);
  intStack.Push(5);
  unsigned int shouldBeFive = intStack.Peek();
  assert(shouldBeFive == 5);

  std::cout << "Testing Testing Pop" << std::endl;
  intStack.Pop();
  intStack.Pop();
  unsigned int shouldBeThree = intStack.Pop();
  assert(shouldBeThree == 3);
  assert(intStack.Size() == 2);

  std::cout << "Testing Clear" << std::endl;
  intStack.Clear();
  assert(intStack.IsEmpty());
  assert(intStack.Size() == 0);

  std::cout << "Stack tests passed!" << std::endl;
  return 0;
}
