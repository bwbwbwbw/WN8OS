#include <terminal.h>

u8 Terminal::make_color(enum VGA_COLOR fg, enum VGA_COLOR bg) {
  return fg | bg << 4;
}

u16 Terminal::make_vgaentry(char c, u8 color) {
  u16 c16 = c;
  u16 color16 = color;
  return c16 | color16 << 8;
}

void Terminal::clear_line(size_t row) {
  u16 blank = make_vgaentry(' ', cur_color);
  for (u16 i = row * VGA_WIDTH; i < VGA_HEIGHT * VGA_WIDTH; ++i) {
    buffer[i] = blank;
  }
  x = 0;
}

void Terminal::scroll() {
  for (size_t row = 0; row < VGA_HEIGHT - 1; ++row) {
    memcpy(buffer + (row + 1) * VGA_WIDTH, buffer + row * VGA_WIDTH, VGA_WIDTH * sizeof(u16));
  }
  clear_line(VGA_HEIGHT - 1);
}

void Terminal::newline() {
  x = 0;
  if (++y == VGA_HEIGHT) {
    scroll();
    y--;
  }
}

void Terminal::setcolor(u8 color) {
  cur_color = color;
}

void Terminal::putentryat(char c, u8 color, size_t x, size_t y) {
  const size_t index = y * VGA_WIDTH + x;
  buffer[index] = make_vgaentry(c, color);
}

void Terminal::putchar(char c) {
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

void Terminal::write(const char * data) {
  size_t datalen = strlen(data);
  for (size_t i = 0; i < datalen; i++) {
    putchar(data[i]);
  }
}

void Terminal::init() {
  y = 0;
  x = 0;
  cur_color = make_color(COLOR_LIGHT_GREY, COLOR_BLACK);
  buffer = (u16 *)0xB8000;
  for (size_t y = 0; y < VGA_HEIGHT; y++) {
    clear_line(y);
  }
}

void Terminal::printf(const char *s, ...) {
  va_list ap;

  char buf[16];
  int i, j, size, buflen, neg;

  unsigned char c;
  int ival;
  unsigned int uival;

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
        ival = va_arg(ap, int);
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
        uival = va_arg(ap, int);
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
        uival = va_arg(ap, int);
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
        uival = va_arg(ap, int);
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
        printf((char *) va_arg(ap, int));
      } 
    } else
      putchar(c);
  }
}
