#ifndef GLIB_DQUEUE_H
#define GLIB_DQUEUE_H

#include "Node.h"
#include "Queue.h"

namespace GLib::DS {

template <typename T>
class DQueue : public Queue<T> {
private:
  unsigned int m_Size;
  DNode<T> *m_pHead;
  DNode<T> *m_pTail;

public:
  DQueue();
  ~DQueue();
  DQueue(DQueue &&other) noexcept;
  DQueue(const DQueue &) = delete;
  DQueue &operator=(const DQueue &) = delete;
  DQueue &operator=(DQueue &&other) noexcept;

  // Queue<T> interface — push to back, pop/peek from front
  void Push(const T &value) override;
  T Pop() override;
  T Peek() const override;
  T PeekBack() const override;
  unsigned int Size() const override;
  bool IsEmpty() const override;
  void Clear() override;
  bool Contains(const T &t) const override;

  // Deque extensions
  void PushFront(const T &value);
  T PopBack();
};

} // namespace GLib::DS

#include "DQueue.tpp"
#endif
