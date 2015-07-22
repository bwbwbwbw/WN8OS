#include <kernel.h>
#include <idt.h>

extern "C"
void kernel_main()
{
  Terminal::init();
  Terminal::printf("WN8OS kernel\n");

  IDT::init();

  __asm__ __volatile__("int $0x3");
  __asm__ __volatile__("int $0x4");
}