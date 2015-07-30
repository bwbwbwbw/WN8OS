#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

#include <runtime/types.h>

namespace Keyboard
{

  typedef u16 scancode_t;
  typedef u16 keycode_t;

  extern const keycode_t KEY_F1;
  extern const keycode_t KEY_F2;
  extern const keycode_t KEY_F3;
  extern const keycode_t KEY_F4;
  extern const keycode_t KEY_F5;
  extern const keycode_t KEY_F6;
  extern const keycode_t KEY_F7;
  extern const keycode_t KEY_F8;
  extern const keycode_t KEY_F9;
  extern const keycode_t KEY_F10;
  extern const keycode_t KEY_F11;
  extern const keycode_t KEY_F12;
  
  extern const keycode_t KEY_INS;
  extern const keycode_t KEY_DEL;
  extern const keycode_t KEY_HOME;
  extern const keycode_t KEY_END;
  extern const keycode_t KEY_PGUP;
  extern const keycode_t KEY_PGDN;
  extern const keycode_t KEY_LFT;
  extern const keycode_t KEY_UP;
  extern const keycode_t KEY_DN;
  extern const keycode_t KEY_RT;
  
  extern const keycode_t KEY_PRNT;
  extern const keycode_t KEY_PAUSE;
  
  extern const keycode_t KEY_LWIN;
  extern const keycode_t KEY_RWIN;
  extern const keycode_t KEY_MENU;
  
  extern const keycode_t KBD_META_ALT;
  extern const keycode_t KBD_META_CTRL;
  extern const keycode_t KBD_META_SHIFT;
  extern const keycode_t KBD_META_ANY;
  extern const keycode_t KBD_META_CAPS;
  extern const keycode_t KBD_META_NUM;
  extern const keycode_t KBD_META_SCRL;
  
  extern const keycode_t RAW1_LEFT_CTRL;
  extern const keycode_t RAW1_LEFT_SHIFT;
  extern const keycode_t RAW1_CAPS_LOCK;
  extern const keycode_t RAW1_LEFT_ALT;
  extern const keycode_t RAW1_RIGHT_ALT;
  extern const keycode_t RAW1_RIGHT_CTRL;
  extern const keycode_t RAW1_RIGHT_SHIFT;
  extern const keycode_t RAW1_SCROLL_LOCK;
  extern const keycode_t RAW1_NUM_LOCK;
  extern const keycode_t RAW1_DEL;

  extern const keycode_t map[];
  extern const keycode_t shift_map[];

  void init();

  bool has_key();
  char getch();
  void getline(char * buffer, size_t len);

}

#endif
