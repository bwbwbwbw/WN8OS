#include "terminal.h"
#include <runtime/itoa.h>
#include <runtime/memory.h>
#include <runtime/string.h>
#include <stdarg.h>
#include <ioport.h>
#include <arch.h>

namespace terminal
{

  const uintptr_t SCREEN_BUFFER_ADDR = 0xB8000;

  const s16 VGA_WIDTH = 80;
  const s16 VGA_HEIGHT = 25;

  s16 x, y;
  u8 cur_color;
  u16 * buffer;

  void update_cursor()
  {
    s16 position = (y * VGA_WIDTH) + x;
    ioport::outb(0x3D4, 0x0F);
    ioport::outb(0x3D5, static_cast<u8>(position & 0xFF));
    ioport::outb(0x3D4, 0x0E);
    ioport::outb(0x3D5, static_cast<u8>((position >> 8) & 0xFF));
  }

  u8 make_color(enum VGA_COLOR fg, enum VGA_COLOR bg)
  {
    return fg | bg << 4;
  }

  u16 make_vgaentry(char c, u8 color)
  {
    u16 c16 = c;
    u16 color16 = color;
    return c16 | color16 << 8;
  }

  void clear_line(s16 row)
  {
    auto blank = make_vgaentry(' ', cur_color);
    for (u16 i = row * VGA_WIDTH; i < VGA_HEIGHT * VGA_WIDTH; ++i) {
      buffer[i] = blank;
    }
    x = 0;
  }

  void scroll()
  {
    for (u16 row = 0; row < VGA_HEIGHT - 1; ++row) {
      memcpy(buffer + row * VGA_WIDTH, buffer + (row + 1) * VGA_WIDTH, VGA_WIDTH * sizeof(u16));
    }
    clear_line(VGA_HEIGHT - 1);
    update_cursor();
  }

  void newline()
  {
    x = 0;
    if (++y == VGA_HEIGHT) {
      scroll();
      y--;
    }
  }

  void setcolor(u8 color)
  {
    cur_color = color;
  }

  void putentryat(char c, u8 color, s16 x, s16 y)
  {
    const s16 index = y * VGA_WIDTH + x;
    buffer[index] = make_vgaentry(c, color);
  }

  // 删除一个字符
  void deletechar()
  {
    x = x - 1;
    if (x < 0) {
      x = VGA_WIDTH - 1;
      y--;
    }
    putentryat(' ', cur_color, x, y);
    update_cursor();
  }

  void putchar(char c)
  {
    if (c == '\r') {
      x = 0;
      update_cursor();
      return;
    }
    if (c == '\n') {
      newline();
      update_cursor();
      return;
    }
    if (c == '\t') {
      x += 8 - (x % 8);
    } else {
      putentryat(c, cur_color, x, y);
    }
    if (++x == VGA_WIDTH) {
      x = 0;
      if (++y == VGA_HEIGHT) {
        scroll();
        y = VGA_HEIGHT - 1;
      }
    }
    update_cursor();
  }

  void write(const char * data)
  {
    auto datalen = strlen(data);
    for (s16 i = 0; i < datalen; i++) {
      putchar(data[i]);
    }
  }

  void init()
  {
    y = 0;
    x = 0;
    cur_color = make_color(COLOR_LIGHT_GREY, COLOR_BLACK);
    buffer = reinterpret_cast<u16 *>(SCREEN_BUFFER_ADDR + KERNEL_VMA_BASE);
    for (s16 y = 0; y < VGA_HEIGHT; y++) {
      clear_line(y);
    }
    update_cursor();
  }

  void printf(const char *s, ...)
  {
    va_list ap;

    char buf[22];
    int i, j, size, buflen, neg;

    unsigned char c;
    s64 ival;
    u64 uival;

    va_start(ap, s);

    while ((c = *s++)) {
      size = 0;
      neg = 0;

      if (c == 0)
        break;
      else if (c == '%') {
        c = *s++;
        if (c >= '0' && c <= '9') {
          size = c - '0';
          c = *s++;
        }

        if (c == 'd') {
          ival = va_arg(ap, s64);
          if (ival < 0) {
            uival = 0 - ival;
            neg++;
          } else
            uival = ival;
          itoa(uival, buf, 10);

          buflen = strlen(buf);
          if (buflen < size)
            for (i = size, j = buflen; i >= 0;
                 i--, j--)
              buf[i] =
                  (j >=
                   0) ? buf[j] : '0';

          if (neg)
            printf("-%s", buf);
          else
            printf(buf);
        }
         else if (c == 'u') {
          uival = va_arg(ap, u64);
          itoa(uival, buf, 10);

          buflen = strlen(buf);
          if (buflen < size)
            for (i = size, j = buflen; i >= 0;
                 i--, j--)
              buf[i] =
                  (j >=
                   0) ? buf[j] : '0';

          printf(buf);
        } else if (c == 'x' || c == 'X') {
          uival = va_arg(ap, u64);
          itoa(uival, buf, 16);

          buflen = strlen(buf);
          if (buflen < size)
            for (i = size, j = buflen; i >= 0;
                 i--, j--)
              buf[i] =
                  (j >=
                   0) ? buf[j] : '0';

          printf("0x%s", buf);
        } else if (c == 'p') {
          uival = va_arg(ap, u64);
          itoa(uival, buf, 16);
          size = 8;

          buflen = strlen(buf);
          if (buflen < size)
            for (i = size, j = buflen; i >= 0;
                 i--, j--)
              buf[i] =
                  (j >=
                   0) ? buf[j] : '0';

          printf("0x%s", buf);
        } else if (c == 's') {
          printf(reinterpret_cast<char *>(va_arg(ap, u64)));
        } 
      } else
        putchar(c);
    }
  }

}
