#include "g-lib/util/StringParser.h"
#include <cassert>
#include <iostream>
#include <vector>

int main() {
  std::cout << "Testing StringParser..." << std::endl;

  std::string input = "Hello,World,Test";
  std::string delimiter = ",";

  GLib::Util::StringParser parser(input, delimiter);

  std::cout << "StringParser Test :: Testing Size" << std::endl;
  assert(parser.getSize() == 3);

  std::cout << "StringParser Test :: Get First Token" << std::endl;
  parser.getFirst();
  assert(parser.getToken() == "Hello");

  std::cout << "StringParser Test :: Get Next Tokens" << std::endl;
  parser.getNext();
  assert(parser.getToken() == "World");
  parser.getNext();
  assert(parser.getToken() == "Test");
  assert(parser.getNext() == false);

  std::cout << "StringParser tests passed!" << std::endl;
  return 0;
}
