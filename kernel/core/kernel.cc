#include "kernel.h"
#include <multiboot.h>
#include <arch.h>

#include <mm/heap.h>
#include <mm/page.h>

#include <simd.h>
#include <interrupt.h>
#include <terminal.h>
#include <keyboard.h>
#include <runtime/abi.h>

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

  SIMD::sse_enable();

  // 初始化内存管理
  Terminal::printf("initializing memory management...\n");
  Terminal::printf("mem_lower = %x KB, mem_upper = %x KB\n", mb_info->mem_lower, mb_info->mem_upper);
  //Heap::init();
  Page::init(mb_info->mem_upper);

  // 设置中断表
  Terminal::printf("Initializing interrupting...\n");
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
  
  // 循环接受键盘输入
  const size_t buffer_size = 128;
  char buffer[buffer_size];

  // 做一个简易的终端
  while (true) {
    Terminal::printf("> ");
    Keyboard::getline(buffer, buffer_size);
    Terminal::newline();

    if (strcmp(buffer, "quit") == 0) {
      return;
    } else if (strcmp(buffer, "help") == 0) {
      Terminal::printf("help: Show this help\n");
      Terminal::printf("version: Show system version\n");
      Terminal::printf("quit: Shutdown the system\n");
    } else if (strcmp(buffer, "version") == 0) {
      Terminal::printf("WN8OS v0.0.1\n");
    } else {
      Terminal::printf("Unknown command: %s\n", buffer);
    }
  }

}
