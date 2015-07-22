#include <kernel.h>
#include <idt.h>
#include <ioport.h>

extern "C"
void kernel_main()
{
  Terminal::init();
  Terminal::printf("WN8OS kernel\n");

  IDT::init();
  Interrupt::remap_pic();
  Interrupt::irq_mask(0xFFFE);
  Interrupt::init_timer(50);
  Interrupt::enable();
}