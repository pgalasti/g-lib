#include "g-lib/util/GeneralScopeGuard.h"
#include <cassert>
#include <iostream>

using namespace GLib::Util;

int main() {
  std::cout << "Testing GeneralScopeGuard..." << std::endl;

  // --- Basic invocation on scope exit ---
  std::cout << "GeneralScopeGuard Test :: Callback invoked on scope exit..." << std::endl;
  {
    bool called = false;
    {
      GeneralScopeGuard gsg([&]{ called = true; });
      assert(!called);
    }
    assert(called);
  }

  // --- Invoked exactly once ---
  std::cout << "GeneralScopeGuard Test :: Callback invoked exactly once..." << std::endl;
  {
    int count = 0;
    {
      GeneralScopeGuard gsg([&]{ ++count; });
    }
    assert(count == 1);
  }

  // --- GSG alias works ---
  std::cout << "GeneralScopeGuard Test :: GSG alias is equivalent..." << std::endl;
  {
    bool called = false;
    {
      auto fn = [&]{ called = true; };
      GSG<decltype(fn)> gsg(fn);
    }
    assert(called);
  }

  // --- Callback observes in-scope mutations ---
  std::cout << "GeneralScopeGuard Test :: Callback observes in-scope mutations..." << std::endl;
  {
    int value = 0;
    int captured = -1;
    {
      GeneralScopeGuard gsg([&]{ captured = value; });
      value = 42;
    }
    assert(captured == 42);
  }

  std::cout << "GeneralScopeGuard tests passed!" << std::endl;
  return 0;
}
