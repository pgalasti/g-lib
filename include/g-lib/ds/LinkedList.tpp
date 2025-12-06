#include <stdexcept>

namespace GLib::DS {

template <typename T>
LinkedList<T>::LinkedList() : m_Size(0), m_pHead(nullptr), m_pTail(nullptr) {}

template <typename T> LinkedList<T>::~LinkedList() { this->Clear(); }

template <typename T>
LinkedList<T>::LinkedList(LinkedList &&other) noexcept
    : m_Size(other.m_Size), m_pHead(other.m_pHead), m_pTail(other.m_pTail) {
  other.m_Size = 0;
  other.m_pHead = nullptr;
  other.m_pTail = nullptr;
}

template <typename T>
LinkedList<T> &LinkedList<T>::operator=(LinkedList &&other) noexcept {

  if (this == &other) {
    return *this;
  }

  Clear();

  m_Size = other.m_Size;
  m_pHead = other.m_pHead;
  m_pTail = other.m_pTail;

  other.m_Size = 0;
  other.m_pHead = nullptr;
  other.m_pTail = nullptr;

  return *this;
}

template <typename T>
void LinkedList<T>::Push(const T &value, LinkedListEnd end) {

  ++m_Size;

  if (m_pTail == nullptr && m_pHead == nullptr) {
    m_pTail = new DNode<T>();
    m_pTail->value = value;
    m_pTail->pRight = m_pTail->pLeft = nullptr;

    m_pHead = m_pTail;
    return;
  }

  DNode<T> *pCursor = new DNode<T>();
  pCursor->value = value;
  pCursor->pLeft = pCursor->pRight = nullptr;
  if (end == LinkedListEnd::END) {
    pCursor->pLeft = m_pTail;
    m_pTail->pRight = pCursor;
    m_pTail = pCursor;
  } else { // LinkedListEnd::Front
    pCursor->pRight = m_pHead;
    m_pHead->pLeft = pCursor;
    m_pHead = pCursor;
  }
}

template <typename T> T LinkedList<T>::Pop(LinkedListEnd end) {
  if (m_pHead == nullptr && m_pTail == nullptr) {
    throw std::runtime_error("Linked List is empty. Unable to pop!");
  }

  --m_Size;

  T value;
  if (m_pHead == m_pTail) {
    value = m_pHead->value;
    delete m_pHead;
    m_pHead = m_pTail = nullptr;
    return value;
  }

  DNode<T> *pCursor = nullptr;

  if (end == LinkedListEnd::END) {
    value = m_pTail->value;
    pCursor = m_pTail->pLeft;
    pCursor->pRight = nullptr;
    delete m_pTail;
    m_pTail = pCursor;
  } else {
    value = m_pHead->value;
    pCursor = m_pHead->pRight;
    pCursor->pLeft = nullptr;
    delete m_pHead;
    m_pHead = pCursor;
  }

  return value;
}

template <typename T> void LinkedList<T>::Clear() {

  DNode<T> *pCursor = m_pHead;

  while (pCursor != nullptr) {
    DNode<T> *pNext = pCursor->pRight;
    delete pCursor;
    pCursor = pNext;
  }
  m_pHead = m_pTail = nullptr;
  m_Size = 0;
}

template <typename T> T LinkedList<T>::Peek(LinkedListEnd end) const {
  if (m_pTail == nullptr && m_pHead == nullptr) {
    throw std::runtime_error("Linked List is empty. Unable to peek!");
  }

  T value;
  if (end == LinkedListEnd::END) {
    value = m_pTail->value;
  } else {
    value = m_pHead->value;
  }

  return value;
}

template <typename T> unsigned int LinkedList<T>::Size() const {
  return m_Size;
}

template <typename T> bool LinkedList<T>::IsEmpty() const {
  return m_Size == 0;
}

template <typename T> bool LinkedList<T>::Contains(const T &t) const {
  if (m_pHead == nullptr) {
    return false;
  }
  if (m_pHead->value == t) {
    return true;
  }

  DNode<T> *pCursor = m_pHead->pRight;
  while (pCursor) {
    if (pCursor->value == t) {
      return true;
    }
    pCursor = pCursor->pRight;
  }

  return false;
}
} // namespace GLib::DS
