#include "g-lib/util/RNG.h"
#include <cassert>
#include <iostream>

using namespace GLib::Util;

int main() {

  std::cout << "RNG :: Testing Construction (min/max params)" << std::endl;
  RNG<int> rng(1, 100);
  assert(rng.GetRange().min == 1);
  assert(rng.GetRange().max == 100);
  assert(rng.IsQueueEmpty());

  std::cout << "RNG :: Testing Construction (Range struct)" << std::endl;
  RNG<int>::Range range{50, 200};
  RNG<int> rng2(range);
  assert(rng2.GetRange().min == 50);
  assert(rng2.GetRange().max == 200);

  std::cout << "RNG :: Testing Generate() stays within range" << std::endl;
  for (int i = 0; i < 1000; ++i) {
    int val = rng.Generate();
    assert(val >= 1 && val <= 100);
  }

  std::cout << "RNG :: Testing Fill(100)" << std::endl;
  rng.Fill(100);
  assert(rng.QueueSize() == 100);

  std::cout << "RNG :: Testing Next() pops from queue" << std::endl;
  unsigned int remaining = 100;
  while (!rng.IsQueueEmpty()) {
    int val = rng.Next();
    assert(val >= 1 && val <= 100);
    --remaining;
    assert(rng.QueueSize() == remaining);
  }
  assert(remaining == 0);

  std::cout << "RNG :: Testing Next() on empty queue throws" << std::endl;
  try {
    rng.Next();
    assert(false);
  } catch (const std::runtime_error &e) {
    std::cout << "Expected error: " << e.what() << std::endl;
  }

  std::cout << "RNG :: Testing Fill() accumulates into existing queue" << std::endl;
  rng.Fill(10);
  rng.Fill(10);
  assert(rng.QueueSize() == 20);

  std::cout << "RNG :: Testing ClearQueue()" << std::endl;
  rng.ClearQueue();
  assert(rng.IsQueueEmpty());

  std::cout << "RNG :: Testing SetRange()" << std::endl;
  rng.SetRange(500, 1000);
  assert(rng.GetRange().min == 500);
  assert(rng.GetRange().max == 1000);
  for (int i = 0; i < 100; ++i) {
    int val = rng.Generate();
    assert(val >= 500 && val <= 1000);
  }

  std::cout << "RNG :: Testing SetRange() with Range struct" << std::endl;
  rng.SetRange(RNG<int>::Range{0, 9});
  rng.Fill(50);
  while (!rng.IsQueueEmpty()) {
    int val = rng.Next();
    assert(val >= 0 && val <= 9);
  }

  std::cout << "RNG :: Testing float type" << std::endl;
  RNG<float> fRng(0.0f, 1.0f);
  fRng.Fill(100);
  assert(fRng.QueueSize() == 100);
  while (!fRng.IsQueueEmpty()) {
    float val = fRng.Next();
    assert(val >= 0.0f && val <= 1.0f);
  }

  std::cout << "RNG tests passed!" << std::endl;
  return 0;
}
