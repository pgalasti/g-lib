#include "StringParser.h"
#include <cassert>
#include <iostream>
#include <vector>

int main() {
  std::cout << "Testing StringParser..." << std::endl;

  std::string input = "Hello,World,Test";
  std::string delimiter = ",";

  GLib::StringParser parser(input, delimiter);

  assert(parser.getSize() == 3);

  parser.getFirst();
  assert(parser.getToken() == "Hello");

  parser.getNext();
  assert(parser.getToken() == "World");

  parser.getNext();
  assert(parser.getToken() == "Test");

  assert(parser.getNext() == false);

  std::cout << "StringParser tests passed!" << std::endl;
  return 0;
}
