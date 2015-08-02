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

  terminal::init();

  // 只允许从 GRUB 启动
  if (mb_magic != MULTIBOOT_BOOTLOADER_MAGIC) {
    terminal::printf("MB_MAGIC = %x, expect %x\n", mb_magic, MULTIBOOT_BOOTLOADER_MAGIC);
    terminal::printf("Assertion failed. Please boot from a multiboot loader!\n");
    return;
  }

  simd::sse_enable();

  // 初始化内存管理
  terminal::printf("initializing memory management...\n");
  terminal::printf("mem_lower = %x KB, mem_upper = %x KB\n", mb_info->mem_lower, mb_info->mem_upper);
  //heap::init();
  page::init(mb_info->mem_upper);

  // 设置中断表
  terminal::printf("Initializing interrupting...\n");
  interrupt::init();
  interrupt::remap_pic();
  
  // We disable timer now
  interrupt::irq_mask(0x1);

  // 初始化键盘处理程序
  terminal::printf("Initializing keyboard...\n");
  keyboard::init();

  terminal::printf("Kernel initialized! Welcome to WN8OS.\n");
  interrupt::enable();

  // 在这里开始调用各种构造函数
  abi::ctors();
  
  //u32 *ptr = (u32*)0xA0000000;
  //u32 do_page_fault = *ptr;
  //interrupt::init_timer(10);
  
  // 循环接受键盘输入
  const size_t buffer_size = 128;
  char buffer[buffer_size];

  // 做一个简易的终端
  while (true) {
    terminal::printf("> ");
    keyboard::getline(buffer, buffer_size);
    terminal::newline();

    if (strcmp(buffer, "quit") == 0) {
      return;
    } else if (strcmp(buffer, "help") == 0) {
      terminal::printf("help: Show this help\n");
      terminal::printf("version: Show system version\n");
      terminal::printf("quit: Shutdown the system\n");
    } else if (strcmp(buffer, "version") == 0) {
      terminal::printf("WN8OS v0.0.1\n");
    } else {
      terminal::printf("Unknown command: %s\n", buffer);
    }
  }

}
