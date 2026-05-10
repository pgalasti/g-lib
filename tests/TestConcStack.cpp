#include "g-lib/ds/concurrency/Stack.h"
#include <cassert>
#include <iostream>
#include <thread>
#include <vector>
#include <atomic>

using GLib::DS::Concurrency::StdWrappedStack;
using GLib::DS::Concurrency::EmptyStackEx;

int main() {

  std::cout << "ConcStack :: Testing initial state" << std::endl;
  StdWrappedStack<int> stack;
  assert(stack.IsEmpty());
  assert(stack.Size() == 0);

  std::cout << "ConcStack :: Testing Push and Size" << std::endl;
  stack.Push(1);
  stack.Push(2);
  stack.Push(3);
  assert(!stack.IsEmpty());
  assert(stack.Size() == 3);

  std::cout << "ConcStack :: Testing Pop (shared_ptr overload)" << std::endl;
  auto ptr = stack.Pop();
  assert(ptr != nullptr);
  assert(*ptr == 3);
  assert(stack.Size() == 2);

  std::cout << "ConcStack :: Testing Pop (reference overload)" << std::endl;
  int val = 0;
  stack.Pop(val);
  assert(val == 2);
  assert(stack.Size() == 1);

  std::cout << "ConcStack :: Testing Pop clears stack" << std::endl;
  stack.Pop(val);
  assert(val == 1);
  assert(stack.IsEmpty());

  std::cout << "ConcStack :: Testing Pop on empty stack (shared_ptr overload)" << std::endl;
  try {
    stack.Pop();
    assert(false);
  } catch (const EmptyStackEx& e) {
    std::cout << "  Expected exception: " << e.what() << std::endl;
  }

  std::cout << "ConcStack :: Testing Pop on empty stack (reference overload)" << std::endl;
  try {
    stack.Pop(val);
    assert(false);
  } catch (const EmptyStackEx& e) {
    std::cout << "  Expected exception: " << e.what() << std::endl;
  }

  std::cout << "ConcStack :: Testing copy constructor" << std::endl;
  stack.Push(10);
  stack.Push(20);
  StdWrappedStack<int> copy(stack);
  assert(copy.Size() == 2);
  assert(stack.Size() == 2);
  int copyVal = 0;
  copy.Pop(copyVal);
  assert(copyVal == 20);
  assert(copy.Size() == 1);
  assert(stack.Size() == 2); // original unmodified

  std::cout << "ConcStack :: Testing Push with string type" << std::endl;
  StdWrappedStack<std::string> strStack;
  strStack.Push(std::string("hello"));
  strStack.Push(std::string("world"));
  assert(strStack.Size() == 2);

  std::cout << "ConcStack :: Testing concurrent producers and consumers" << std::endl;
  StdWrappedStack<int> concStack;
  constexpr int kItemsPerThread = 1000;
  constexpr int kThreads = 4;
  std::atomic<int> consumed{0};

  // Producers
  std::vector<std::thread> producers;
  for (int t = 0; t < kThreads; ++t) {
    producers.emplace_back([&]() {
      for (int i = 0; i < kItemsPerThread; ++i)
        concStack.Push(i);
    });
  }
  for (auto& th : producers) th.join();
  assert(concStack.Size() == kThreads * kItemsPerThread);

  // Consumers
  std::vector<std::thread> consumers;
  for (int t = 0; t < kThreads; ++t) {
    consumers.emplace_back([&]() {
      for (int i = 0; i < kItemsPerThread; ++i) {
        int v = 0;
        concStack.Pop(v);
        consumed.fetch_add(1, std::memory_order_relaxed);
      }
    });
  }
  for (auto& th : consumers) th.join();
  assert(concStack.IsEmpty());
  assert(consumed.load() == kThreads * kItemsPerThread);

  std::cout << "ConcStack tests passed!" << std::endl;
  return 0;
}
