#include "g-lib/ds/Node.h"
#include <cassert>
#include <iostream>

using namespace GLib::DS;

int main() {
  std::cout << "SNode Test :: Test creating a node"<< std::endl;
  SNode<int> node1;
  node1.value = 10;
  node1.pNextNode = nullptr;
  assert(node1.value == 10);
  assert(node1.pNextNode == nullptr);

  std::cout << "SNode Test :: Test initial root attach"<< std::endl;
  SNode<int> *root = nullptr;
  root = attach_node(root, &node1);
  assert(root == &node1);
  assert(root->value == 10);
  assert(root->pNextNode == nullptr);

  std::cout << "SNode Test :: Test attach to existing root"<< std::endl;
  SNode<int> node2;
  node2.value = 20;
  node2.pNextNode = nullptr;

  attach_node(root, &node2);
  assert(root->pNextNode == &node2);
  assert(root->pNextNode->value == 20);

  std::cout << "SNode Test :: Test attach to chain"<< std::endl;
  SNode<int> node3;
  node3.value = 30;
  node3.pNextNode = nullptr;

  attach_node(root, &node3);
  assert(root->pNextNode->pNextNode == &node3);
  assert(root->pNextNode->pNextNode->value == 30);

  std::cout << "SNode tests passed!" << std::endl;
  return 0;
}
