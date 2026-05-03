#ifndef G_LIB_LEAK_DETECTOR_ACTIVATE_H
#define G_LIB_LEAK_DETECTOR_ACTIVATE_H

// Include this header in EXACTLY ONE translation unit to activate leak tracking.
//
// Tracking is only wired up when ENABLE_LEAK_DETECTOR is defined, either via
// a compiler flag (-DENABLE_LEAK_DETECTOR) or before this include:
//
//   #define ENABLE_LEAK_DETECTOR
//   #include "g-lib/memory/LeakDetectorActivate.h"
//
// Without ENABLE_LEAK_DETECTOR the include is a no-op, so you can leave it in
// your main.cpp unconditionally and toggle via the build system.
//
// LeakDetector.h is safe to include from any TU — it provides the Usage() API
// with zero overhead when tracking is inactive.

#ifdef ENABLE_LEAK_DETECTOR

#include "g-lib/memory/LeakDetector.h"
#include <cstdlib>
#include <new>

// These replacement functions must be non-inline (strong) symbols so they
// override the standard library's versions at link time. Portable across
// GCC, Clang, and Apple Clang without any compiler-specific attributes.

void* operator new(std::size_t mem) {
  void* ptr = std::malloc(mem);
  if (!ptr) throw std::bad_alloc{};
  GLib::Memory::LeakDetector::instance()
    .Record(GLib::Memory::LeakDetector::AllocType::Credit, mem);
  return ptr;
}

void operator delete(void* ptr) noexcept {
  std::free(ptr);
}

void operator delete(void* ptr, std::size_t mem) noexcept {
  if (!ptr) return;
  GLib::Memory::LeakDetector::instance()
    .Record(GLib::Memory::LeakDetector::AllocType::Debit, mem);
  std::free(ptr);
}

void* operator new[](std::size_t mem) {
  void* ptr = std::malloc(mem);
  if (!ptr) throw std::bad_alloc{};
  GLib::Memory::LeakDetector::instance()
    .Record(GLib::Memory::LeakDetector::AllocType::Credit, mem);
  return ptr;
}

void operator delete[](void* ptr) noexcept {
  std::free(ptr);
}

void operator delete[](void* ptr, std::size_t mem) noexcept {
  if (!ptr) return;
  GLib::Memory::LeakDetector::instance()
    .Record(GLib::Memory::LeakDetector::AllocType::Debit, mem);
  std::free(ptr);
}

#endif // ENABLE_LEAK_DETECTOR
#endif // G_LIB_LEAK_DETECTOR_ACTIVATE_H
