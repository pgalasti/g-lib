#ifndef GLIB_SEEK_H
#define GLIB_SEEK_H

namespace GLib::DS {
  template <typename T>
  class Seek {
    public:
      virtual ~Seek() = default;

      virtual bool Contains(const T& t) const = 0;
  };
}

#endif
