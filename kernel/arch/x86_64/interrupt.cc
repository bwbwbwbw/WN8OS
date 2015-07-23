#include <interrupt.h>
#include <terminal.h>

namespace Interrupt
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

  interrupt_handler_t handlers[INTERRUPT_MAX];

  void init()
  {
    // 未注册的都为 0
    for (u32 i = 0; i < INTERRUPT_MAX; ++i) {
      handlers[i] = 0;
    }
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
    IOport::outb(0x20, 0x20);
  }

  void eoi_slave()
  {
    IOport::outb(0xA0, 0x20);
    IOport::outb(0x20, 0x20);
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

  /**
   * 屏蔽一部分 IRQ
   */
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
   * 注册中断处理程序，目前只支持一个中断向量注册一个函数
   */
  void register_handler(interrupt_vector_t vector, interrupt_handler_t handler)
  {
    handlers[vector] = handler;
  }

}

extern "C"
void interrupt_handler(
  Interrupt::interrupt_vector_t vector,
  u64 error_code,
  registers_t *registers,
  Interrupt::interrupt_stack_frame_t *interrupt_stack_frame
  )
{
  // 调用中断处理函数
  if (Interrupt::handlers[vector] != 0) {
    Interrupt::interrupt_handler_t handler = Interrupt::handlers[vector];
    handler(vector, error_code, registers, interrupt_stack_frame);
  }

  // RPC 中断？
  if (vector == 63)
  {
    /*  interrupt_rpc_t rpc = (interrupt_rpc_t)(void*)registers->rdi;
      rpc(registers, interrupt_stack_frame,
          registers->rsi, registers->rdx, registers->rcx, registers->r8, registers->r9);
    */
  }

  // 时钟中断？
  if (vector == Interrupt::INT_IRQ0) // timer interrupt
  {
    // Do nothing
    //Terminal::printf("tick: %x\n", ++tick);
  }

  // 对于 PIC，需要发送 EOI
  if (vector >= Interrupt::INT_IRQ0 && vector <= Interrupt::INT_IRQ7) {
    Interrupt::eoi_master();
  } else if (vector >= Interrupt::INT_IRQ8 && vector <= Interrupt::INT_IRQ15) {
    Interrupt::eoi_slave();
  }
}