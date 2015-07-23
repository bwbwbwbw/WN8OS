#include <kernel.h>

#include <mm/malloc.h>

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

  // 初始化内存管理
  Terminal::printf("initializing memory management...\n");
  Terminal::printf("mem_lower = %x KB, mem_upper = %x KB\n", mb_info->mem_lower, mb_info->mem_upper);
  MM::init();
  Heap::init();
  Page::init(mb_info->mem_upper);

  // 设置中断表
  Terminal::printf("Initializing interrupting...\n");
  IDT::init();
  Interrupt::init();
  Interrupt::remap_pic();
  
  // We disable timer now
  Interrupt::irq_mask(0x1);

  // 初始化键盘处理程序
  Terminal::printf("Initializing keyboard...\n");
  Keyboard::init();

  Terminal::printf("Kernel initialized! Welcome to WN8OS.\n");
  Interrupt::enable();

  // 在这里开始调用各种构造函数
  ABI::ctors();
  //u32 *ptr = (u32*)0xA0000000;
  //u32 do_page_fault = *ptr;
  //Interrupt::init_timer(10);
  Interrupt::enable();

  while (true) {
    char ch = Keyboard::getch();
    Terminal::printf("%x\n", ch);
  }

}
