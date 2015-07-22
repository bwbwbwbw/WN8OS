#include <kernel.h>
#include <idt.h>

extern "C"
void kernel_main()
{
  Terminal::init();
  Terminal::printf("WN8OS kernel\n");

  IDT::init();
  IDT::remap_pic();
  IDT::irq_mask(0xFFFE);
  IDT::init_timer(50);  // 50Hz

  __asm__ __volatile__("int $0x3");
  __asm__ __volatile__("int $0x4");
}