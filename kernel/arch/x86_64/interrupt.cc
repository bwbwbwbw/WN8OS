#include "interrupt.h"

#include <arch.h>
#include <runtime/memory.h>
#include <terminal.h>
#include <ioport.h>

extern void * ISR_HANDLERS;  // in isr.S

namespace interrupt
{

  const interrupt_vector_t INT_IRQ0  = ((interrupt_vector_t)0x20);
  const interrupt_vector_t INT_IRQ1  = (INT_IRQ0 + 1);
  const interrupt_vector_t INT_IRQ2  = (INT_IRQ0 + 2);
  const interrupt_vector_t INT_IRQ3  = (INT_IRQ0 + 3);
  const interrupt_vector_t INT_IRQ4  = (INT_IRQ0 + 4);
  const interrupt_vector_t INT_IRQ5  = (INT_IRQ0 + 5);
  const interrupt_vector_t INT_IRQ6  = (INT_IRQ0 + 6);
  const interrupt_vector_t INT_IRQ7  = (INT_IRQ0 + 7);
  const interrupt_vector_t INT_IRQ8  = ((interrupt_vector_t)0x28);
  const interrupt_vector_t INT_IRQ9  = (INT_IRQ8 + 1);
  const interrupt_vector_t INT_IRQ10 = (INT_IRQ8 + 2);
  const interrupt_vector_t INT_IRQ11 = (INT_IRQ8 + 3);
  const interrupt_vector_t INT_IRQ12 = (INT_IRQ8 + 4);
  const interrupt_vector_t INT_IRQ13 = (INT_IRQ8 + 5);
  const interrupt_vector_t INT_IRQ14 = (INT_IRQ8 + 6);
  const interrupt_vector_t INT_IRQ15 = (INT_IRQ8 + 7);

  // IDT
  idt_entry_t idt_entries[256];
  idt_ptr_t   idt_ptr;

  // interrupt handlers
  interrupt_handler_t handlers[INTERRUPT_MAX];

  /**
   * 设置一个中断向量
   */
  void idt_set_gate(u8 vector, uintptr_t base, u16 select, u8 flags)
  {
    //terminal::printf("set idt #%d: %x\n", vector, base);

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
  void idt_flush(idt_ptr_t * idt_ptr)
  {
    __asm__ __volatile__ ("lidt (%[idtr])" : : [idtr]"p"(reinterpret_cast<uintptr_t>(idt_ptr)));
  }

  /**
   * 初始化：中断描述表，0 ~ INTERRUPT_MAX - 1 中断都将最后由 interrupt_handler 处理
   */
  void init()
  {
    // 重置所有中断处理函数指针
    for (u32 i = 0; i < INTERRUPT_MAX; ++i) {
      handlers[i] = 0;
    }

    idt_ptr.limit = sizeof(idt_entry_t) * 256 - 1;
    idt_ptr.base  = reinterpret_cast<uintptr_t>(&idt_entries);

    memset(&idt_entries, 0, sizeof(idt_entry_t)*256);

    for (u16 i = 0; i < INTERRUPT_MAX; ++i) {
      uintptr_t handlerAddress = reinterpret_cast<uintptr_t>(&ISR_HANDLERS) + i * INTERRUPT_ISR_ALIGN;
      // handlerAddress 为线性地址，需要转换成 CS_KERNEL:偏移地址
      idt_set_gate(i, handlerAddress, CS_KERNEL, 0x8E);
    }

    idt_flush(&idt_ptr);
  }

  /**
   * 开启中断
   */
  void enable()
  {
    __asm__ __volatile__ ("sti");
  }

  /**
   * 屏蔽中断
   */
  void disable()
  {
    __asm__ __volatile__ ("cli");
  }

  void eoi_master()
  {
    ioport::outb(0x20, 0x20);
  }

  void eoi_slave()
  {
    ioport::outb(0xA0, 0x20);
    ioport::outb(0x20, 0x20);
  }

  /**
   * 这些魔数是啥查手册去 ╮(╯▽╰)╭
   */
  void remap_pic()
  {
    ioport::outb(0x20, 0x11);
    ioport::outb(0xA0, 0x11);
    ioport::outb(0x21, INT_IRQ0);
    ioport::outb(0xA1, INT_IRQ8);
    ioport::outb(0x21, 0x04);
    ioport::outb(0xA1, 0x02);
    ioport::outb(0x21, 0x01);
    ioport::outb(0xA1, 0x01);
  }

  /**
   * 屏蔽一部分 IRQ
   */
  void irq_mask(u16 mask)
  {
    ioport::outb(0x21, static_cast<u8>(mask & 0xff));
    ioport::outb(0xA1, static_cast<u8>(mask >> 8));
  }

  /**
   * 设置时钟中断频率
   */
  void init_timer(u32 frequency)
  {
    u32 divisor = 1193180 / frequency;
  
    ioport::outb(0x43, 0x36);
  
    u8 l = static_cast<u8>(divisor & 0xFF);
    u8 h = static_cast<u8>((divisor >> 8) & 0xFF);
  
    ioport::outb(0x40, l);
    ioport::outb(0x40, h);
  }

  /**
   * 注册中断处理程序，目前只支持一个中断向量注册一个函数
   */
  void register_handler(interrupt_vector_t vector, interrupt_handler_t handler)
  {
    handlers[vector] = handler;
  }

}

extern "C"
void interrupt_handler(
  interrupt::interrupt_vector_t vector,
  u64 error_code,
  registers_t *registers,
  interrupt::interrupt_stack_frame_t *interrupt_stack_frame
  )
{
  // 调用中断处理函数
  if (interrupt::handlers[vector] != 0) {
    interrupt::interrupt_handler_t handler = interrupt::handlers[vector];
    handler(vector, error_code, registers, interrupt_stack_frame);
  }

  // RPC 中断？
  if (vector == 63) {
    /*  interrupt_rpc_t rpc = (interrupt_rpc_t)(void*)registers->rdi;
      rpc(registers, interrupt_stack_frame,
          registers->rsi, registers->rdx, registers->rcx, registers->r8, registers->r9);
    */
  }

  // 时钟中断？
  if (vector == interrupt::INT_IRQ0) {
    // Do nothing
    //terminal::printf("tick: %x\n", ++tick);
  }

  // 对于 PIC，需要发送 EOI
  if (vector >= interrupt::INT_IRQ0 && vector <= interrupt::INT_IRQ7) {
    interrupt::eoi_master();
  } else if (vector >= interrupt::INT_IRQ8 && vector <= interrupt::INT_IRQ15) {
    interrupt::eoi_slave();
  }
}
