#ifndef _IO_H_
#define _IO_H_

#include <runtime/types.h>

namespace IOport
{
  typedef u16 ioport_t;
  
  void outb(ioport_t port, u8 val);
  void outw(ioport_t port, u16 val);
  void outl(ioport_t port, u32 val);
  u8 inb(ioport_t port);
  u16 inw(ioport_t port);
  u32 inl(ioport_t port);
}

#endif