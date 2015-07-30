#include "ioport.h"

namespace IOport
{

  void outb(ioport_t port, u8 val)
  {
    __asm__ __volatile__ ("outb %0, %1" :: "a"(val), "Nd" (port));
  }

  void outw(ioport_t port, u16 val)
  {
    __asm__ __volatile__ ("outw %0, %1" :: "a"(val), "Nd" (port));
  }

  void outl(ioport_t port, u32 val)
  {
    __asm__ __volatile__ ("outl %0, %1" :: "a"(val), "Nd" (port));
  }

  u8 inb(ioport_t port)
  {
      u8 val;
      __asm__ __volatile__ ("inb %1, %0" : "=a"(val) : "Nd"(port));
      return val;
  }

  u16 inw(ioport_t port)
  {
      u16 val;
      __asm__ __volatile__ ("inw %1, %0" : "=a"(val) : "Nd"(port));
      return val;
  }

  u32 inl(ioport_t port)
  {
      u32 val;
      __asm__ __volatile__ ("inl %1, %0" : "=a"(val) : "Nd"(port));
      return val;
  }

}
