#ifndef G_DELETER_H
#define G_DELETER_H

#include <atomic>
#include <chrono>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>

namespace GLib::Core {

  class Deleter {
  public:
    explicit Deleter(std::chrono::milliseconds interval);
    ~Deleter();

    Deleter(const Deleter&) = delete;
    Deleter& operator=(const Deleter&) = delete;

    // Enqueue ptr for deletion on the next flush cycle.
    template <typename T>
    void Schedule(T* ptr) {
      std::lock_guard<std::mutex> lock(m_Mutex);
      m_Queue.push([ptr] { delete ptr; });
    }

    // Immediately delete everything currently queued without waiting for the next cycle.
    void Flush();

  private:
    void Run();

    std::chrono::milliseconds    m_Interval;
    std::queue<std::function<void()>> m_Queue;
    std::mutex                   m_Mutex;
    std::atomic<bool>            m_Running{false};
    std::thread                  m_Thread;
  };

}


#endif // G_DELETER_H
