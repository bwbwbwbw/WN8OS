#include <kernel.h>

extern "C"
void kernel_main(u32 mb_magic, multiboot_info_t * mb_info)
{
  Terminal::init();

  // 只允许从 GRUB 启动
  if (mb_magic != MULTIBOOT_BOOTLOADER_MAGIC) {
    Terminal::printf("MB_MAGIC = %x, expect %x\n", mb_magic, MULTIBOOT_BOOTLOADER_MAGIC);
    Terminal::printf("Assertion failed. Please boot from a multiboot loader!\n");
    return;
  }

  Terminal::printf("mem_lower = %x KB, mem_upper = %x KB\n", mb_info->mem_lower, mb_info->mem_upper);

  Terminal::printf("Initializing interrupting...");
  IDT::init();
  Interrupt::remap_pic();
  Interrupt::irq_mask(0xFFFE);
  //Interrupt::init_timer(50);
  //Interrupt::enable();
}