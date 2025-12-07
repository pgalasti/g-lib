#include <stdexcept>

namespace GLib::DS {

template <typename T> Stack<T>::Stack() : m_Size(0), m_pTop(nullptr) {}

template <typename T> Stack<T>::~Stack() { Clear(); }

template <typename T> Stack<T>::Stack(Stack &&other) noexcept {
  m_Size = other.m_Size;
  m_pTop = other.m_pTop;

  other.m_Size = 0;
  other.m_pTop = nullptr;
}

template <typename T> Stack<T> &Stack<T>::operator=(Stack &&other) noexcept {
  if (this == &other) {
    return *this;
  }

  Clear();

  m_Size = other.m_Size;
  m_pTop = other.m_pTop;

  other.m_Size = 0;
  other.m_pTop = nullptr;

  return *this;
}

template <typename T> void Stack<T>::Push(const T &value) {

  ++m_Size;

  SNode<T> *pNewNode = new SNode<T>();
  pNewNode->value = value;
  pNewNode->pNextNode = m_pTop;

  m_pTop = pNewNode;
}

template <typename T> T Stack<T>::Pop() {

  if (IsEmpty()) {
    throw std::runtime_error("Stack is empty. Unable to pop!");
  }

  --m_Size;

  const T value = m_pTop->value;
  SNode<T> *pCursor = m_pTop->pNextNode;
  delete m_pTop;

  m_pTop = pCursor;

  return value;
}

template <typename T> T Stack<T>::Peek() const {

  if (IsEmpty()) {
    throw std::runtime_error("Stack is empty. Unable to peek!");
  }

  return m_pTop->value;
}

template <typename T> unsigned int Stack<T>::Size() const { return m_Size; }

template <typename T> bool Stack<T>::IsEmpty() const { return m_Size == 0; }

template <typename T> void Stack<T>::Clear() {
  while (m_pTop) {
    Pop();
  }
}

} // namespace GLib::DS
