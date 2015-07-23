#ifndef _TERMINAL_H_
#define _TERMINAL_H_

#include <stdarg.h>
#include <runtime/itoa.h>
#include <runtime/types.h>
#include <runtime/memory.h>
#include <runtime/string.h>

namespace Terminal
{

  static const size_t VGA_WIDTH = 80;
  static const size_t VGA_HEIGHT = 25;

  enum VGA_COLOR {
    COLOR_BLACK = 0,
    COLOR_BLUE = 1,
    COLOR_GREEN = 2,
    COLOR_CYAN = 3,
    COLOR_RED = 4,
    COLOR_MAGENTA = 5,
    COLOR_BROWN = 6,
    COLOR_LIGHT_GREY = 7,
    COLOR_DARK_GREY = 8,
    COLOR_LIGHT_BLUE = 9,
    COLOR_LIGHT_GREEN = 10,
    COLOR_LIGHT_CYAN = 11,
    COLOR_LIGHT_RED = 12,
    COLOR_LIGHT_MAGENTA = 13,
    COLOR_LIGHT_BROWN = 14,
    COLOR_WHITE = 15,
  };

  void init();
  void clear_line(size_t row);
  void scroll();
  void newline();
  void setcolor(u8 color);
  void putentryat(char c, u8 color, size_t x, size_t y);
  void putchar(char c);
  void write(const char * data);
  void printf(const char *s, ...);
  u8 make_color(enum VGA_COLOR fg, enum VGA_COLOR bg);
  u16 make_vgaentry(char c, u8 color);

};

#endif
