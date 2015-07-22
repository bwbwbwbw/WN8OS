#include <interrupt.h>
#include <terminal.h>

u64 tick = 0;

namespace Interrupt
{
  void enable()
  {
    __asm__ __volatile__ ("sti");
  }

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

}

extern "C"
void interrupt_handler(
  Interrupt::interrupt_vector_t vector,
  u64 error_code,
  registers_t *registers,
  Interrupt::interrupt_stack_frame_t *interrupt_stack_frame
  )
{
  (void)vector;
  (void)error_code;
  (void)registers;
  (void)interrupt_stack_frame;

  //Terminal::printf("interrupt: %x, error_code: %x\n", vector, error_code);

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
    Terminal::printf("tick: %x\n", ++tick);
  }

  // 对于 PIC，需要发送 EOI
  if (vector >= Interrupt::INT_IRQ0 && vector <= Interrupt::INT_IRQ7) {
    Interrupt::eoi_master();
  } else if (vector >= Interrupt::INT_IRQ8 && vector <= Interrupt::INT_IRQ15) {
    Interrupt::eoi_slave();
  }
}