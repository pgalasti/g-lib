#include "g-lib/ds/Node.h"
#include <cassert>
#include <iostream>

using namespace GLib::DS;

int main() {
  std::cout << "Testing SNode..." << std::endl;

  // Test 1: Create a node
  SNode<int> node1;
  node1.value = 10;
  node1.pNextNode = nullptr;
  assert(node1.value == 10);
  assert(node1.pNextNode == nullptr);

  // Test 2: attach_node to nullptr
  SNode<int> *root = nullptr;
  root = attach_node(root, &node1);
  assert(root == &node1);
  assert(root->value == 10);
  assert(root->pNextNode == nullptr);

  // Test 3: attach_node to existing root
  SNode<int> node2;
  node2.value = 20;
  node2.pNextNode = nullptr;

  attach_node(root, &node2);
  assert(root->pNextNode == &node2);
  assert(root->pNextNode->value == 20);

  // Test 4: attach_node to chain (depth > 1)
  SNode<int> node3;
  node3.value = 30;
  node3.pNextNode = nullptr;

  attach_node(root, &node3);
  assert(root->pNextNode->pNextNode == &node3);
  assert(root->pNextNode->pNextNode->value == 30);

  std::cout << "SNode tests passed!" << std::endl;
  return 0;
}
