#include "g-lib/util/IDGenerator.h"
#include <cassert>
#include <iostream>

int main() {
  std::cout << "Testing IDGenerator..." << std::endl;

  GLib::Util::IDGenerator<int> gen(100);

  std::cout << "IDGenerator Test :: Testing Next ID Generation" << std::endl;
  assert(gen.GetNextId() == 100);
  assert(gen.GetNextId() == 101);

  std::cout << "IDGenerator Test :: Testing Release ID" << std::endl;
  gen.Release(100);

  std::cout << "IDGenerator tests passed!" << std::endl;
  return 0;
}
