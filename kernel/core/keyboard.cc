#include <keyboard.h>

#include <interrupt.h>
#include <terminal.h>

namespace Keyboard
{

  // 修改自：http://geezer.osdevbrasil.net/osd/kbd/kbd.c
  
  const keycode_t KEY_F1            = 0x80;
  const keycode_t KEY_F2            = (KEY_F1 + 1);
  const keycode_t KEY_F3            = (KEY_F2 + 1);
  const keycode_t KEY_F4            = (KEY_F3 + 1);
  const keycode_t KEY_F5            = (KEY_F4 + 1);
  const keycode_t KEY_F6            = (KEY_F5 + 1);
  const keycode_t KEY_F7            = (KEY_F6 + 1);
  const keycode_t KEY_F8            = (KEY_F7 + 1);
  const keycode_t KEY_F9            = (KEY_F8 + 1);
  const keycode_t KEY_F10           = (KEY_F9 + 1);
  const keycode_t KEY_F11           = (KEY_F10 + 1);
  const keycode_t KEY_F12           = (KEY_F11 + 1);
  
  const keycode_t KEY_INS           = 0x90;
  const keycode_t KEY_DEL           = (KEY_INS + 1);
  const keycode_t KEY_HOME          = (KEY_DEL + 1);
  const keycode_t KEY_END           = (KEY_HOME + 1);
  const keycode_t KEY_PGUP          = (KEY_END + 1);
  const keycode_t KEY_PGDN          = (KEY_PGUP + 1);
  const keycode_t KEY_LFT           = (KEY_PGDN + 1);
  const keycode_t KEY_UP            = (KEY_LFT + 1);
  const keycode_t KEY_DN            = (KEY_UP + 1);
  const keycode_t KEY_RT            = (KEY_DN + 1);
  
  const keycode_t KEY_PRNT          = (KEY_RT + 1);
  const keycode_t KEY_PAUSE         = (KEY_PRNT + 1);
  
  const keycode_t KEY_LWIN          = (KEY_PAUSE + 1);
  const keycode_t KEY_RWIN          = (KEY_LWIN + 1);
  const keycode_t KEY_MENU          = (KEY_RWIN + 1);
  
  const keycode_t KBD_META_ALT      = 0x0200;
  const keycode_t KBD_META_CTRL     = 0x0400;
  const keycode_t KBD_META_SHIFT    = 0x0800;
  const keycode_t KBD_META_ANY      = (KBD_META_ALT | KBD_META_CTRL | KBD_META_SHIFT);
  const keycode_t KBD_META_CAPS     = 0x1000;
  const keycode_t KBD_META_NUM      = 0x2000;
  const keycode_t KBD_META_SCRL     = 0x4000;
  
  const keycode_t RAW1_LEFT_CTRL    = 0x1D;
  const keycode_t RAW1_LEFT_SHIFT   = 0x2A;
  const keycode_t RAW1_CAPS_LOCK    = 0x3A;
  const keycode_t RAW1_LEFT_ALT     = 0x38;
  const keycode_t RAW1_RIGHT_ALT    = 0x38;
  const keycode_t RAW1_RIGHT_CTRL   = 0x1D;
  const keycode_t RAW1_RIGHT_SHIFT  = 0x36;
  const keycode_t RAW1_SCROLL_LOCK  = 0x46;
  const keycode_t RAW1_NUM_LOCK     = 0x45;
  const keycode_t RAW1_DEL          = 0x53;

  const keycode_t map[] =
  {
    /* 00 */0,  0x1B, '1',  '2',  '3',  '4',  '5',  '6',
    /* 08 */'7',  '8',  '9',  '0',  '-',  '=',  '\b', '\t',
    /* 10 */'q',  'w',  'e',  'r',  't',  'y',  'u',  'i',
    /* 1Dh is left Ctrl */
    /* 18 */'o',  'p',  '[',  ']',  '\n', 0,  'a',  's',
    /* 20 */'d',  'f',  'g',  'h',  'j',  'k',  'l',  ';',
    /* 2Ah is left Shift */
    /* 28 */'\'', '`',  0,  '\\', 'z',  'x',  'c',  'v',
    /* 36h is right Shift */
    /* 30 */'b',  'n',  'm',  ',',  '.',  '/',  0,  0,
    /* 38h is left Alt, 3Ah is Caps Lock */
    /* 38 */0,  ' ',  0,  KEY_F1, KEY_F2, KEY_F3, KEY_F4, KEY_F5,
    /* 45h is Num Lock, 46h is Scroll Lock */
    /* 40 */KEY_F6, KEY_F7, KEY_F8, KEY_F9, KEY_F10,0,  0,  KEY_HOME,
    /* 48 */KEY_UP, KEY_PGUP,'-', KEY_LFT,'5',  KEY_RT, '+',  KEY_END,
    /* 50 */KEY_DN, KEY_PGDN,KEY_INS,KEY_DEL,0, 0,  0,  KEY_F11,
    /* 58 */KEY_F12
  };
  
  const keycode_t shift_map[] =
  {
    /* 00 */0,  0x1B, '!',  '@',  '#',  '$',  '%',  '^',
    /* 08 */'&',  '*',  '(',  ')',  '_',  '+',  '\b', '\t',
    /* 10 */'Q',  'W',  'E',  'R',  'T',  'Y',  'U',  'I',
    /* 1Dh is left Ctrl */
    /* 18 */'O',  'P',  '{',  '}',  '\n', 0,  'A',  'S',
    /* 20 */'D',  'F',  'G',  'H',  'J',  'K',  'L',  ':',
    /* 2Ah is left Shift */
    /* 28 */'"',  '~',  0,  '|',  'Z',  'X',  'C',  'V',
    /* 36h is right Shift */
    /* 30 */'B',  'N',  'M',  '<',  '>',  '?',  0,  0,
    /* 38h is left Alt, 3Ah is Caps Lock */
    /* 38 */0,  ' ',  0,  KEY_F1, KEY_F2, KEY_F3, KEY_F4, KEY_F5,
    /* 45h is Num Lock, 46h is Scroll Lock */
    /* 40 */KEY_F6, KEY_F7, KEY_F8, KEY_F9, KEY_F10,0,  0,  KEY_HOME,
    /* 48 */KEY_UP, KEY_PGUP,'-', KEY_LFT,'5',  KEY_RT, '+',  KEY_END,
    /* 50 */KEY_DN, KEY_PGDN,KEY_INS,KEY_DEL,0, 0,  0,  KEY_F11,
    /* 58 */KEY_F12
  };

  // TODO: 这货需要改进
  keycode_t scancode_to_keycode(scancode_t code)
  {
    static bool saw_break_code;
    static keycode_t kbd_status;

    keycode_t temp;

    /* check for break code (i.e. a key is released) */
    if (code >= 0x80)
    {
      saw_break_code = true;
      code &= 0x7F;
    }

    /* the only break codes we're interested in are Shift, Ctrl, Alt */
    if (saw_break_code)
    {
      if(code == RAW1_LEFT_ALT || code == RAW1_RIGHT_ALT)
        kbd_status &= ~KBD_META_ALT;
      else if(code == RAW1_LEFT_CTRL || code == RAW1_RIGHT_CTRL)
        kbd_status &= ~KBD_META_CTRL;
      else if(code == RAW1_LEFT_SHIFT || code == RAW1_RIGHT_SHIFT)
        kbd_status &= ~KBD_META_SHIFT;
      saw_break_code = false;
      return -1;
    }

    /* it's a make code: check the "meta" keys, as above */
    if (code == RAW1_LEFT_ALT || code == RAW1_RIGHT_ALT)
    {
      kbd_status |= KBD_META_ALT;
      return -1;
    }
    if (code == RAW1_LEFT_CTRL || code == RAW1_RIGHT_CTRL)
    {
      kbd_status |= KBD_META_CTRL;
      return -1;
    }
    if (code == RAW1_LEFT_SHIFT || code == RAW1_RIGHT_SHIFT)
    {
      kbd_status |= KBD_META_SHIFT;
      return -1;
    }
    /* no conversion if Alt pressed */
    if (kbd_status & KBD_META_ALT)
      return code;
    /* convert A-Z[\]^_ to control chars */
    if (kbd_status & KBD_META_CTRL)
    {
      if(code >= sizeof(map) / sizeof(map[0]))
        return -1;
      temp = map[code];
      if(temp >= 'a' && temp <= 'z')
        return temp - 'a';
      if(temp >= '[' && temp <= '_')
        return temp - '[' + 0x1B;
      return -1;
    }
    /* convert raw scancode to ASCII */
    if (kbd_status & KBD_META_SHIFT)
    {
      /* ignore invalid scan codes */
      if (code >= sizeof(shift_map) / sizeof(shift_map[0]))
        return -1;
      temp = shift_map[code];
      /* defective keyboard? non-US keyboard? more than 104 keys? */
      if(temp == 0)
        return -1;
      /* caps lock? */
      if (kbd_status & KBD_META_CAPS)
      {
        if (temp >= 'A' && temp <= 'Z')
          temp = map[code];
      }
    }
    else
    {
      if (code >= sizeof(map) / sizeof(map[0]))
        return -1;
      temp = map[code];
      if(temp == 0)
        return -1;
      if(kbd_status & KBD_META_CAPS)
      {
        if(temp >= 'a' && temp <= 'z')
          temp = shift_map[code];
      }
    }
    return temp;
  }

  const size_t KEY_BUFFER_SIZE = 16;
  volatile keycode_t key_buffer[KEY_BUFFER_SIZE];
  volatile s64 key_buffer_len = -1;

  keycode_t keycodeBuf = -1;

  void handler(Interrupt::interrupt_vector_t vector,
    u64 error_code,
    registers_t *registers,
    Interrupt::interrupt_stack_frame_t *interrupt_stack_frame
    )
  {
    unsigned char status = IOport::inb(0x64);
    unsigned char scancode = IOport::inb(0x60);
    keycode_t code = scancode_to_keycode(scancode);
    if (code != (keycode_t)-1) {
      if (key_buffer_len + 1 < KEY_BUFFER_SIZE) {
        key_buffer[++key_buffer_len] = code;
      }
    }
  }

  void init()
  {
    Interrupt::register_handler(Interrupt::INT_IRQ1, &handler);
  }

  bool has_key()
  {
    return key_buffer_len > -1;
  }

  char getch()
  {
    while (true) {
      Interrupt::disable();
      if (has_key()) {
        break;
      }
      Interrupt::enable();
    }
    Interrupt::disable();
    keycode_t ret = key_buffer[0];
    for (size_t i = 0; i < key_buffer_len; ++i) {
      key_buffer[i] = key_buffer[i + 1];
    }
    key_buffer_len--;
    Interrupt::enable();
    if (ret > 0x20 && ret <= 0x7e) {
      // 如果是可见字符，则显示出来
      Terminal::putchar(ret);
    }
    return (char)ret;
  }

  void getline(char * buffer, size_t len)
  {
    size_t pos = 0;
    char ch = getch();
    while (ch != 0xA && pos + 1 < len) {
      if (ch > 0x20 && ch <= 0x7e) {
        buffer[pos++] = ch;
      } else if (ch == 0x8) {
        if (pos > 0) {
          pos--;
          Terminal::deletechar();
        }
      }
      ch = getch();
    }
    buffer[pos] = '\0';
  }

}
