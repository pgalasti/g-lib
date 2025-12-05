namespace GLib::DS {
  template<typename T>
  struct SNode {
    T value;
    SNode* pNextNode;
  };

  template<typename T>
  SNode<T>* attach_node(SNode<T>* pRoot, SNode<T>* pNode)
  {
    if (!pRoot) { return pNode; }

    if (!pRoot->pNextNode) {
        pRoot->pNextNode = pNode;
        return pRoot;
    }

    attach_node(pRoot->pNextNode, pNode);
    return pRoot;
  }

  template<typename T>
  struct DNode {
    T value;
    DNode* pLeft;
    DNode* pRight;
  };
  
  // Usage:
  // pRoot = attachNode(pRoot, pNewNode, [](const int& a, const int& b) { return a < b; });
  template<typename T, typename Comp>
  DNode<T>* attach_node(DNode<T>* pRoot, DNode<T>* pNode, Comp comFn)
  {
    if(!pRoot) { return pNode; }

    if(comFn(pNode->value, pRoot->value)) {
        pRoot->pLeft = attach_node(pRoot->pLeft, pNode, comFn);
	return pRoot;
    }

    pRoot->pRight = attach_node(pRoot->pRight, pNode, comFn);
    return pRoot;
  }
}
