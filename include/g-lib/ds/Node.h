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
}
