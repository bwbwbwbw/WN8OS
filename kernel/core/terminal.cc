#include <terminal.h>

namespace Terminal {

  size_t x, y;
  u8 cur_color;
  u16 * buffer;

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

  void clear_line(size_t row)
  {
    u16 blank = make_vgaentry(' ', cur_color);
    for (u16 i = row * VGA_WIDTH; i < VGA_HEIGHT * VGA_WIDTH; ++i) {
      buffer[i] = blank;
    }
    x = 0;
  }

  void scroll()
  {
    for (size_t row = 0; row < VGA_HEIGHT - 1; ++row) {
      memcpy(buffer + row * VGA_WIDTH, buffer + (row + 1) * VGA_WIDTH, VGA_WIDTH * sizeof(u16));
    }
    clear_line(VGA_HEIGHT - 1);
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

  void putentryat(char c, u8 color, size_t x, size_t y)
  {
    const size_t index = y * VGA_WIDTH + x;
    buffer[index] = make_vgaentry(c, color);
  }

  void putchar(char c)
  {
    if (c == '\r') {
      x = 0;
      return;
    }
    if (c == '\n') {
      newline();
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
  }

  void write(const char * data)
  {
    size_t datalen = strlen(data);
    for (size_t i = 0; i < datalen; i++) {
      putchar(data[i]);
    }
  }

  void init()
  {
    y = 0;
    x = 0;
    cur_color = make_color(COLOR_LIGHT_GREY, COLOR_BLACK);
    buffer = (u16 *)0xB8000;
    for (size_t y = 0; y < VGA_HEIGHT; y++) {
      clear_line(y);
    }
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
          printf((char *) va_arg(ap, u64));
        } 
      } else
        putchar(c);
    }
  }

}
