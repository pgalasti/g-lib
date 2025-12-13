#include "g-lib/util/Stopwatch.h"
#include <cassert>
#include <chrono>
#include <iostream>
#include <thread>
#include <string>

int main() {
  std::cout << "Testing Stopwatch..." << std::endl;

  std::cout << "Stopwatch Test :: Testing Start and Current..." << std::endl;
  GLib::Util::Stopwatch sw("TestTimer");
  sw.Start();
  std::this_thread::sleep_for(std::chrono::milliseconds(10));
  assert(sw.Current() > 0);

  std::cout << "Stopwatch Test :: Testing Tick..." << std::endl;
  std::this_thread::sleep_for(std::chrono::milliseconds(10));
  auto tick1 = sw.Tick();
  assert(tick1 > 0);

  std::this_thread::sleep_for(std::chrono::milliseconds(10));
  auto tick2 = sw.Tick();
  assert(tick2 > 0);

  assert(sw.Current() > tick2);

  std::cout << "Stopwatch Test :: Testing Reset..." << std::endl;
  sw.Reset();
  auto afterReset = sw.Current();
  assert(afterReset >= 0);

  std::cout << "Stopwatch Test :: Testing Label..." << std::endl;
  const std::string label = sw.GetLabel();
  assert(label == "TestTimer");

  std::cout << "Stopwatch tests passed!" << std::endl;
  return 0;
}
