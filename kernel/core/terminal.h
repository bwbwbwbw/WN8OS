#ifndef _TERMINAL_H_
#define _TERMINAL_H_

#include <runtime/types.h>

namespace Terminal
{

  extern const s16 VGA_WIDTH;
  extern const s16 VGA_HEIGHT;

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
  void clear_line(s16 row);
  void scroll();
  void newline();
  void setcolor(u8 color);
  void putentryat(char c, u8 color, s16 x, s16 y);
  void putchar(char c);
  void write(const char * data);
  void printf(const char *s, ...);
  u8 make_color(enum VGA_COLOR fg, enum VGA_COLOR bg);
  u16 make_vgaentry(char c, u8 color);
  void deletechar();

}

#endif
