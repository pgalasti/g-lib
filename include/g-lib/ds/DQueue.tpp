#include <stdexcept>

namespace GLib::DS {

template <typename T>
DQueue<T>::DQueue() : m_Size(0), m_pHead(nullptr), m_pTail(nullptr) {}

template <typename T> DQueue<T>::~DQueue() { Clear(); }

template <typename T>
DQueue<T>::DQueue(DQueue &&other) noexcept
    : m_Size(other.m_Size), m_pHead(other.m_pHead), m_pTail(other.m_pTail) {
  other.m_Size = 0;
  other.m_pHead = nullptr;
  other.m_pTail = nullptr;
}

template <typename T>
DQueue<T> &DQueue<T>::operator=(DQueue &&other) noexcept {
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

template <typename T> void DQueue<T>::Push(const T &value) {
  ++m_Size;

  DNode<T> *pNode = new DNode<T>();
  pNode->value = value;
  pNode->pLeft = pNode->pRight = nullptr;

  if (m_pTail == nullptr) {
    m_pHead = m_pTail = pNode;
    return;
  }

  pNode->pLeft = m_pTail;
  m_pTail->pRight = pNode;
  m_pTail = pNode;
}

template <typename T> void DQueue<T>::PushFront(const T &value) {
  ++m_Size;

  DNode<T> *pNode = new DNode<T>();
  pNode->value = value;
  pNode->pLeft = pNode->pRight = nullptr;

  if (m_pHead == nullptr) {
    m_pHead = m_pTail = pNode;
    return;
  }

  pNode->pRight = m_pHead;
  m_pHead->pLeft = pNode;
  m_pHead = pNode;
}

template <typename T> T DQueue<T>::Pop() {
  if (m_pHead == nullptr) {
    throw std::runtime_error("DQueue is empty. Unable to pop!");
  }

  --m_Size;

  const T value = m_pHead->value;

  if (m_pHead == m_pTail) {
    delete m_pHead;
    m_pHead = m_pTail = nullptr;
    return value;
  }

  DNode<T> *pNext = m_pHead->pRight;
  pNext->pLeft = nullptr;
  delete m_pHead;
  m_pHead = pNext;

  return value;
}

template <typename T> T DQueue<T>::PopBack() {
  if (m_pTail == nullptr) {
    throw std::runtime_error("DQueue is empty. Unable to pop!");
  }

  --m_Size;

  const T value = m_pTail->value;

  if (m_pHead == m_pTail) {
    delete m_pTail;
    m_pHead = m_pTail = nullptr;
    return value;
  }

  DNode<T> *pPrev = m_pTail->pLeft;
  pPrev->pRight = nullptr;
  delete m_pTail;
  m_pTail = pPrev;

  return value;
}

template <typename T> void DQueue<T>::Clear() {
  DNode<T> *pCursor = m_pHead;
  while (pCursor != nullptr) {
    DNode<T> *pNext = pCursor->pRight;
    delete pCursor;
    pCursor = pNext;
  }
  m_pHead = m_pTail = nullptr;
  m_Size = 0;
}

template <typename T> T DQueue<T>::Peek() const {
  if (m_pHead == nullptr) {
    throw std::runtime_error("DQueue is empty. Unable to peek!");
  }
  return m_pHead->value;
}

template <typename T> T DQueue<T>::PeekBack() const {
  if (m_pTail == nullptr) {
    throw std::runtime_error("DQueue is empty. Unable to peek!");
  }
  return m_pTail->value;
}

template <typename T> unsigned int DQueue<T>::Size() const { return m_Size; }

template <typename T> bool DQueue<T>::IsEmpty() const { return m_Size == 0; }

template <typename T> bool DQueue<T>::Contains(const T &t) const {
  DNode<T> *pCursor = m_pHead;
  while (pCursor) {
    if (pCursor->value == t) {
      return true;
    }
    pCursor = pCursor->pRight;
  }
  return false;
}

} // namespace GLib::DS
