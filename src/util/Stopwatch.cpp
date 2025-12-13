#include "g-lib/util/Stopwatch.h"

namespace GLib::Util {

Stopwatch::Stopwatch(const std::string &label)
    : m_Label(label), m_Start(clock::now()) {
  this->Reset();
}

Stopwatch::~Stopwatch() {}

void Stopwatch::Start() { m_LastTick = clock::now(); }

void Stopwatch::Reset() { m_Start = clock::now(); }

std::string Stopwatch::GetLabel() const { return m_Label; }

} // namespace GLib::Util
