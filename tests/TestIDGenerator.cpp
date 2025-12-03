#include "IDGenerator.h"
#include <cassert>
#include <iostream>

int main() {
  std::cout << "Testing IDGenerator..." << std::endl;

  GLib::IDGenerator<int> gen(100);

  // Test initial values
  assert(gen.GetNextId() == 100);
  assert(gen.GetNextId() == 101);

  // Test Release
  gen.Release(100);
  // Queue: [102, ..., 199, 100]

  // Consume until we get 100 back
  for (int i = 102; i < 200; ++i) {
    assert(gen.GetNextId() == i);
  }

  assert(gen.GetNextId() == 100);

  std::cout << "IDGenerator tests passed!" << std::endl;
  return 0;
}
