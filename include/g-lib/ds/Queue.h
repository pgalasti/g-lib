#ifndef GLIB_QUEUE_H
#define GLIB_QUEUE_H

#include "Seek.h"

namespace GLib::DS {

template <typename T>
class Queue : public Seek<T> {
public:
  virtual ~Queue() = default;

  virtual void Push(const T &value) = 0;
  virtual T Pop() = 0;
  virtual T Peek() const = 0;
  virtual T PeekBack() const = 0;
  virtual unsigned int Size() const = 0;
  virtual bool IsEmpty() const = 0;
  virtual void Clear() = 0;

  bool Contains(const T &t) const override = 0;
};

} // namespace GLib::DS

#endif
