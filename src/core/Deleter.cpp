#include "g-lib/core/Deleter.h"

namespace GLib::Core {

  Deleter::Deleter(std::chrono::milliseconds interval)
    : m_Interval(interval), m_Running(true)
  {
    m_Thread = std::thread(&Deleter::Run, this);
  }

  Deleter::~Deleter() {
    m_Running = false;
    m_Thread.join();
    Flush();
  }

  void Deleter::Flush() {
    std::queue<std::function<void()>> local;
    {
      std::lock_guard<std::mutex> lock(m_Mutex);
      std::swap(local, m_Queue);
    }
    while (!local.empty()) {
      local.front()();
      local.pop();
    }
  }

  void Deleter::Run() {
    while (m_Running) {
      std::this_thread::sleep_for(m_Interval);
      Flush();
    }
  }

}
