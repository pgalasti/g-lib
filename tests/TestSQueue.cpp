#include "g-lib/ds/SQueue.h"
#include "g-lib/ds/Queue.h"
#include "g-lib/ds/Seek.h"
#include <cassert>
#include <iostream>

using namespace GLib::DS;

int main() {

  std::cout << "SQueue :: Testing Initialization" << std::endl;
  SQueue<int> queue;
  assert(queue.Size() == 0);
  assert(queue.IsEmpty());

  std::cout << "SQueue :: Testing Push and FIFO order" << std::endl;
  queue.Push(10);
  queue.Push(20);
  queue.Push(30);
  assert(queue.Size() == 3);
  assert(queue.Peek() == 10);
  assert(queue.PeekBack() == 30);

  std::cout << "SQueue :: Testing Pop (FIFO)" << std::endl;
  assert(queue.Pop() == 10);
  assert(queue.Size() == 2);
  assert(queue.Peek() == 20);
  assert(queue.Pop() == 20);
  assert(queue.Pop() == 30);
  assert(queue.IsEmpty());

  std::cout << "SQueue :: Testing Pop on empty" << std::endl;
  try {
    queue.Pop();
    assert(false);
  } catch (const std::runtime_error &e) {
    std::cout << "Expected error: " << e.what() << std::endl;
  }

  std::cout << "SQueue :: Testing Clear" << std::endl;
  queue.Push(1);
  queue.Push(2);
  queue.Push(3);
  queue.Clear();
  assert(queue.Size() == 0);
  assert(queue.IsEmpty());

  std::cout << "SQueue :: Testing std::string type" << std::endl;
  SQueue<std::string> strQueue;
  strQueue.Push("Hello");
  strQueue.Push("World");
  assert(strQueue.Size() == 2);
  assert(strQueue.Pop() == "Hello");
  assert(strQueue.Pop() == "World");

  std::cout << "SQueue :: Testing Seek Interface" << std::endl;
  SQueue<int> seekQueue;
  seekQueue.Push(1);
  seekQueue.Push(2);
  seekQueue.Push(3);
  Seek<int> &seek = seekQueue;
  assert(seek.Contains(2));
  assert(!seek.Contains(99));

  std::cout << "SQueue :: Testing Queue base pointer" << std::endl;
  Queue<int> *pQueue = new SQueue<int>();
  pQueue->Push(5);
  pQueue->Push(6);
  assert(pQueue->Peek() == 5);
  assert(pQueue->PeekBack() == 6);
  assert(pQueue->Pop() == 5);
  delete pQueue;

  std::cout << "SQueue :: Testing Move Constructor" << std::endl;
  SQueue<int> src;
  src.Push(7);
  src.Push(8);
  SQueue<int> dst(std::move(src));
  assert(dst.Size() == 2);
  assert(src.IsEmpty());
  assert(dst.Pop() == 7);

  std::cout << "SQueue tests passed!" << std::endl;
  return 0;
}
