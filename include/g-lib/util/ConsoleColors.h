#include <iostream>
#include <ostream>

// Probably doesn't work for Windows

namespace GLib::Util {

  enum class Color {
      // Reset
      Reset         = 0,

      // Text styles
      Bold          = 1,
      Dim           = 2,
      Italic        = 3,
      Underline     = 4,
      Blink         = 5,
      Reverse       = 7,
      Strikethrough = 9,

      // Foreground colors
      Black         = 30,
      Red           = 31,
      Green         = 32,
      Yellow        = 33,
      Blue          = 34,
      Magenta       = 35,
      Cyan          = 36,
      White         = 37,

      // Bright foreground colors
      BrightBlack   = 90,
      BrightRed     = 91,
      BrightGreen   = 92,
      BrightYellow  = 93,
      BrightBlue    = 94,
      BrightMagenta = 95,
      BrightCyan    = 96,
      BrightWhite   = 97,

      // Background colors
      BgBlack       = 40,
      BgRed         = 41,
      BgGreen       = 42,
      BgYellow      = 43,
      BgBlue        = 44,
      BgMagenta     = 45,
      BgCyan        = 46,
      BgWhite       = 47,

      // Bright background colors
      BgBrightBlack   = 100,
      BgBrightRed     = 101,
      BgBrightGreen   = 102,
      BgBrightYellow  = 103,
      BgBrightBlue    = 104,
      BgBrightMagenta = 105,
      BgBrightCyan    = 106,
      BgBrightWhite   = 107,
  };

  inline std::ostream& operator<<(std::ostream& os, Color c) {
      return os << "\033[" << static_cast<int>(c) << "m";
  }
}
