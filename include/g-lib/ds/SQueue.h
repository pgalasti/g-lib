#ifndef GLIB_SQUEUE_H
#define GLIB_SQUEUE_H

#include "Node.h"
#include "Queue.h"

namespace GLib::DS {

template <typename T>
class SQueue : public Queue<T> {
private:
  unsigned int m_Size;
  DNode<T> *m_pHead;
  DNode<T> *m_pTail;

public:
  SQueue();
  ~SQueue();
  SQueue(SQueue &&other) noexcept;
  SQueue(const SQueue &) = delete;
  SQueue &operator=(const SQueue &) = delete;
  SQueue &operator=(SQueue &&other) noexcept;

  void Push(const T &value) override;
  T Pop() override;
  void Clear() override;

  T Peek() const override;
  T PeekBack() const override;
  unsigned int Size() const override;
  bool IsEmpty() const override;

  bool Contains(const T &t) const override;
};

} // namespace GLib::DS

#include "SQueue.tpp"
#endif
