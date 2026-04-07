#include "g-lib/ds/HashMap.h"
#include <cassert>
#include <iostream>
#include <stdexcept>
#include <string>

using namespace GLib::DS;

int main() {

  std::cout << "HashMap :: Testing Construction" << std::endl;
  Hashmap<std::string, int> map;
  assert(map.Size() == 0);
  assert(map.IsEmpty());

  std::cout << "HashMap :: Testing Insert and Contains" << std::endl;
  map.Insert("one", 1);
  map.Insert("two", 2);
  map.Insert("three", 3);
  assert(map.Contains("one"));
  assert(map.Contains("two"));
  assert(map.Contains("three"));
  assert(!map.Contains("four"));

  std::cout << "HashMap :: Testing Size after inserts" << std::endl;
  assert(map.Size() == 3);

  std::cout << "HashMap :: Testing Get" << std::endl;
  assert(map.Get("one") == 1);
  assert(map.Get("two") == 2);
  assert(map.Get("three") == 3);

  std::cout << "HashMap :: Testing Get on missing key throws" << std::endl;
  try {
    map.Get("four");
    assert(false);
  } catch (const std::runtime_error &e) {
    std::cout << "Expected error: " << e.what() << std::endl;
  }

  std::cout << "HashMap :: Testing Insert overwrites existing key" << std::endl;
  map.Insert("one", 100);
  assert(map.Get("one") == 100);
  assert(map.Size() == 3);

  std::cout << "HashMap :: Testing Clear" << std::endl;
  map.Clear();
  assert(map.Size() == 0);
  assert(map.IsEmpty());
  assert(!map.Contains("one"));
  assert(!map.Contains("two"));

  std::cout << "HashMap :: Testing Move Constructor" << std::endl;
  Hashmap<std::string, int> map2;
  map2.Insert("a", 10);
  map2.Insert("b", 20);
  Hashmap<std::string, int> map3(std::move(map2));
  assert(map3.Size() == 2);
  assert(map3.Contains("a"));
  assert(map3.Get("a") == 10);
  assert(map3.Contains("b"));
  assert(map3.Get("b") == 20);

  std::cout << "HashMap :: Testing integer keys" << std::endl;
  Hashmap<int, std::string> intMap;
  intMap.Insert(1, "one");
  intMap.Insert(2, "two");
  intMap.Insert(3, "three");
  assert(intMap.Size() == 3);
  assert(intMap.Get(1) == "one");
  assert(intMap.Get(3) == "three");
  assert(intMap.Contains(2));
  assert(!intMap.Contains(99));

  std::cout << "HashMap tests passed!" << std::endl;
  return 0;
}
