#ifndef GLIB_STACK_H
#define GLIB_STACK_H

#include "Node.h"

namespace GLib::DS {

template <typename T> class Stack {
private:
  unsigned int m_Size;
  SNode<T> *m_pTop;

public:
  Stack();
  ~Stack();
  Stack(Stack &&other) noexcept;
  Stack(const Stack &) = delete;
  Stack &operator=(const Stack &) = delete;
  Stack &operator=(Stack &&other) noexcept;

  void Push(const T &t);
  T Pop();
  T Peek() const;
  unsigned int Size() const;
  bool IsEmpty() const;
  void Clear();
};
} // namespace GLib::DS

#include "Stack.tpp"
#endif
