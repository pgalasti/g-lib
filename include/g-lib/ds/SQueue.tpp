#include <stdexcept>

namespace GLib::DS {

template <typename T>
SQueue<T>::SQueue() : m_Size(0), m_pHead(nullptr), m_pTail(nullptr) {}

template <typename T> SQueue<T>::~SQueue() { Clear(); }

template <typename T>
SQueue<T>::SQueue(SQueue &&other) noexcept
    : m_Size(other.m_Size), m_pHead(other.m_pHead), m_pTail(other.m_pTail) {
  other.m_Size = 0;
  other.m_pHead = nullptr;
  other.m_pTail = nullptr;
}

template <typename T>
SQueue<T> &SQueue<T>::operator=(SQueue &&other) noexcept {
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

template <typename T> void SQueue<T>::Push(const T &value) {
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

template <typename T> T SQueue<T>::Pop() {
  if (m_pHead == nullptr) {
    throw std::runtime_error("SQueue is empty. Unable to pop!");
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

template <typename T> void SQueue<T>::Clear() {
  DNode<T> *pCursor = m_pHead;
  while (pCursor != nullptr) {
    DNode<T> *pNext = pCursor->pRight;
    delete pCursor;
    pCursor = pNext;
  }
  m_pHead = m_pTail = nullptr;
  m_Size = 0;
}

template <typename T> T SQueue<T>::Peek() const {
  if (m_pHead == nullptr) {
    throw std::runtime_error("SQueue is empty. Unable to peek!");
  }
  return m_pHead->value;
}

template <typename T> T SQueue<T>::PeekBack() const {
  if (m_pTail == nullptr) {
    throw std::runtime_error("SQueue is empty. Unable to peek!");
  }
  return m_pTail->value;
}

template <typename T> unsigned int SQueue<T>::Size() const { return m_Size; }

template <typename T> bool SQueue<T>::IsEmpty() const { return m_Size == 0; }

template <typename T> bool SQueue<T>::Contains(const T &t) const {
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
