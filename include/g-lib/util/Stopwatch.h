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

      template <typename Duration = std::chrono::microseconds>
      std::int64_t Current() const;

      template <typename Duration = std::chrono::microseconds>
      std::int64_t Tick();
      
   private:
      clock::time_point m_Start{};
      clock::time_point m_LastTick{};
  };

}


#endif
