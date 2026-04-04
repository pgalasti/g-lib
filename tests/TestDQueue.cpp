#include "g-lib/ds/DQueue.h"
#include "g-lib/ds/Queue.h"
#include "g-lib/ds/Seek.h"
#include <cassert>
#include <iostream>

using namespace GLib::DS;

int main() {

  std::cout << "DQueue :: Testing Initialization" << std::endl;
  DQueue<int> dq;
  assert(dq.Size() == 0);
  assert(dq.IsEmpty());

  std::cout << "DQueue :: Testing Push (back) and Pop (front)" << std::endl;
  dq.Push(10);
  dq.Push(20);
  dq.Push(30);
  assert(dq.Size() == 3);
  assert(dq.Peek() == 10);
  assert(dq.PeekBack() == 30);
  assert(dq.Pop() == 10);
  assert(dq.Size() == 2);

  std::cout << "DQueue :: Testing PushFront" << std::endl;
  dq.PushFront(5);
  assert(dq.Size() == 3);
  assert(dq.Peek() == 5);
  assert(dq.PeekBack() == 30);

  std::cout << "DQueue :: Testing PopBack" << std::endl;
  assert(dq.PopBack() == 30);
  assert(dq.Size() == 2);
  assert(dq.PeekBack() == 20);

  // Current state: [5, 20]

  std::cout << "DQueue :: Testing interleaved push/pop both ends" << std::endl;
  dq.PushFront(1);
  dq.Push(99);
  // State: [1, 5, 20, 99]
  assert(dq.Size() == 4);
  assert(dq.Pop() == 1);
  assert(dq.PopBack() == 99);
  assert(dq.Pop() == 5);
  assert(dq.PopBack() == 20);
  assert(dq.IsEmpty());

  std::cout << "DQueue :: Testing Pop on empty" << std::endl;
  try {
    dq.Pop();
    assert(false);
  } catch (const std::runtime_error &e) {
    std::cout << "Expected error: " << e.what() << std::endl;
  }

  std::cout << "DQueue :: Testing PopBack on empty" << std::endl;
  try {
    dq.PopBack();
    assert(false);
  } catch (const std::runtime_error &e) {
    std::cout << "Expected error: " << e.what() << std::endl;
  }

  std::cout << "DQueue :: Testing Clear" << std::endl;
  dq.Push(1);
  dq.Push(2);
  dq.PushFront(0);
  dq.Clear();
  assert(dq.Size() == 0);
  assert(dq.IsEmpty());

  std::cout << "DQueue :: Testing std::string type" << std::endl;
  DQueue<std::string> strDQ;
  strDQ.Push("World");
  strDQ.PushFront("Hello");
  assert(strDQ.Pop() == "Hello");
  assert(strDQ.Pop() == "World");

  std::cout << "DQueue :: Testing Seek Interface" << std::endl;
  DQueue<int> seekDQ;
  seekDQ.Push(10);
  seekDQ.Push(20);
  seekDQ.PushFront(5);
  Seek<int> &seek = seekDQ;
  assert(seek.Contains(5));
  assert(seek.Contains(20));
  assert(!seek.Contains(99));

  std::cout << "DQueue :: Testing Queue base pointer" << std::endl;
  Queue<int> *pQueue = new DQueue<int>();
  pQueue->Push(100);
  pQueue->Push(200);
  assert(pQueue->Pop() == 100);
  delete pQueue;

  std::cout << "DQueue :: Testing Move Constructor" << std::endl;
  DQueue<int> src;
  src.Push(1);
  src.PushFront(0);
  DQueue<int> dst(std::move(src));
  assert(dst.Size() == 2);
  assert(src.IsEmpty());
  assert(dst.Pop() == 0);

  std::cout << "DQueue tests passed!" << std::endl;
  return 0;
}
