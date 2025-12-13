#ifndef G_STOPWATCH_H
#define G_STOPWATCH_H

#include <string>
#include <chrono>
#include <cstdint>

namespace GLib::Util {

  class Stopwatch {
    public:

      // Defining this here to avoid verbose lines
      using clock = std::chrono::steady_clock;

      Stopwatch(const std::string& label);
      ~Stopwatch();
      
      void Start();
      void Reset();
      std::string GetLabel() const;

      template <typename Duration = std::chrono::microseconds>
      std::int64_t Current() const {
        return std::chrono::duration_cast<Duration>(clock::now() - m_Start).count();
      }

      template <typename Duration = std::chrono::microseconds>
      std::int64_t Tick() {
	auto now = clock::now();
        auto delta = std::chrono::duration_cast<Duration>(now - m_LastTick).count();
        m_LastTick = now;
        return delta;
      }
      
   private:
      std::string m_Label;
      clock::time_point m_Start{};
      clock::time_point m_LastTick{};
  };

}


#endif
