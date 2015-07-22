#include <idt.h>

extern void * ISR_HANDLERS;

namespace IDT {

  idt_entry_t idt_entries[256];
  idt_ptr_t   idt_ptr;

  /**
   * 初始化中断描述表，0 ~ INTERRUPT_MAX - 1 中断都将最后由 interrupt_handler 处理
   */
  void init()
  {
    idt_ptr.limit = sizeof(idt_entry_t) * 256 - 1;
    idt_ptr.base  = (uintptr_t)&idt_entries;

    //Terminal::printf("interrupt_max = %d\n", INTERRUPT_MAX);
    //Terminal::printf("&ISR_HANDLERS - KERNEL_VMA_BASE = %x\n", ((u64)&ISR_HANDLERS)-KERNEL_VMA_BASE);

    memset(&idt_entries, 0, sizeof(idt_entry_t)*256);

    for (u16 i = 0; i < INTERRUPT_MAX; ++i)
    {
      uintptr_t handlerAddress = (uintptr_t)&ISR_HANDLERS + i * INTERRUPT_ISR_ALIGN;
      // handlerAddress 为线性地址，需要转换成 CS_KERNEL:偏移地址
      set_gate(i, handlerAddress - KERNEL_VMA_BASE, CS_KERNEL, 0x8E);
    }

    flush(&idt_ptr);
  }

  /**
   * 这些魔数是啥查手册去 ╮(╯▽╰)╭
   */
  void remap_pic()
  {
    IOport::outb(0x20, 0x11);
    IOport::outb(0xA0, 0x11);
    IOport::outb(0x21, INT_IRQ0);
    IOport::outb(0xA1, INT_IRQ8);
    IOport::outb(0x21, 0x04);
    IOport::outb(0xA1, 0x02);
    IOport::outb(0x21, 0x01);
    IOport::outb(0xA1, 0x01);
  }

  void irq_mask(u16 mask)
  {
    IOport::outb(0x21, (u8)(mask & 0xff));
    IOport::outb(0xA1, (u8)(mask >> 8));
  }

  /**
   * 设置时钟中断频率
   */
  void init_timer(u32 frequency)
  {
    u32 divisor = 1193180 / frequency;
  
    IOport::outb(0x43, 0x36);
  
    u8 l = (u8)(divisor & 0xFF);
    u8 h = (u8)((divisor >> 8) & 0xFF);
  
    IOport::outb(0x40, l);
    IOport::outb(0x40, h);
  }

  /**
   * 设置一个中断向量
   */
  void set_gate(u8 vector, u64 base, u16 select, u8 flags)
  {
    //Terminal::printf("set idt #%d: %x\n", vector, base);

    idt_entries[vector].base_lo = base & 0xFFFF;
    idt_entries[vector].base_mi = (base >> 16) & 0xFFFF;
    idt_entries[vector].base_hi = (base >> 32);

    idt_entries[vector].select  = select;
    idt_entries[vector].zero_1  = 0;
    idt_entries[vector].zero_2  = 0;

    idt_entries[vector].flags   = flags | 0x60;
  }

  /**
   * 更新 IDT 寄存器
   */
  void flush(idt_ptr_t * idt_ptr)
  {
    __asm__ __volatile__ ("lidt (%[idtr])" : : [idtr]"p"((uintptr_t)idt_ptr));
  }

}
