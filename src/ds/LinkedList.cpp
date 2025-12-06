#include "g-lib/ds/LinkedList.h"
#include "g-lib/ds/Node.h"

namespace GLib::DS {

template <typename T> LinkedList<T>::LinkedList() : m_Size(0), m_pHead(nullptr), m_pTail(nullptr) {}

template <typename T> LinkedList<T>::~LinkedList() {
  SNode* pCursor = m_pHead;

  while(pCursor) {
    SNode* pNext = pCursor->pNextNode;
    delete pCursor;
    pCursor = pNext;
  }
}

template <typename T> LinkedList<T>::LinkedList(LinkedList &&other) noexcept {
}

template <typename T>
LinkedList<T> &LinkedList<T>::operator=(LinkedList &&other) noexcept {
  // TODO: Implement
  return *this;
}

template <typename T>
void LinkedList<T>::Push(const T &value, LinkedListEnd end) {
  // TODO: Implement
}

template <typename T> T LinkedList<T>::Pop(LinkedListEnd end) {
  // TODO: Implement
  return T();
}

template <typename T> void LinkedList<T>::Clear() {
  // TODO: Implement
}

template <typename T> T LinkedList<T>::Peek(LinkedListEnd end) const {
  // TODO: Implement
  return T();
}

template <typename T> unsigned int LinkedList<T>::size() const {
  // TODO: Implement
  return 0;
}

// Explicit instantiation example
template class LinkedList<int>;

} // namespace GLib::DS
