#ifndef GLIB_CONC_STACK_H
#define GLIB_CONC_STACK_H

#include <exception>
#include <mutex>
#include <stack>
#include <memory>

namespace GLib::DS::Concurrency {

struct EmptyStackEx : std::exception {
  const char* what() const noexcept { return "Stack is empty."; }
};

template <typename T>
class StdWrappedStack {

  mutable std::mutex m_Mtx;
  std::stack<T> m_StdStack;
public:
  StdWrappedStack()                                        = default;
  StdWrappedStack& operator=(const StdWrappedStack& other) = delete;
  ~StdWrappedStack()                                       = default;

  StdWrappedStack(const StdWrappedStack& other) {
    std::lock_guard guard(other.m_Mtx);
    m_StdStack = other.m_StdStack;
  }

  size_t Size() const {
    std::lock_guard guard(m_Mtx);
    return m_StdStack.size();
  }
  bool IsEmpty() const {
    return Size() == 0;
  }

  void Push(T value) {
    std::lock_guard guard(m_Mtx);
    m_StdStack.push(std::move(value));
  }

  void Pop(T& value) {
    std::lock_guard guard(m_Mtx);
    if(m_StdStack.empty()) throw EmptyStackEx();
    value = m_StdStack.top();
    m_StdStack.pop();
  }
  std::shared_ptr<T> Pop() {
    std::lock_guard guard(m_Mtx);
    if(m_StdStack.empty()) throw EmptyStackEx();
    const std::shared_ptr<T> ptr(std::make_shared<T>(m_StdStack.top()));
    m_StdStack.pop();
    return ptr;
  }

};

} // GLib::DS::Concurrency


#endif // GLIB_CONC_STACK_H
