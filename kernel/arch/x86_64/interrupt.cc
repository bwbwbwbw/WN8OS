#include <interrupt.h>
#include <terminal.h>

extern "C"
void interrupt_handler(
  interrupt_vector_t vector,
  u64 error_code,
  registers_t *registers,
  interrupt_stack_frame_t *interrupt_stack_frame
  )
{
  (void)vector;
  (void)error_code;
  (void)registers;
  (void)interrupt_stack_frame;

  Terminal::printf("interrupt: %x, error_code: %x\n", vector, error_code);

  // RPC 中断？
  if (vector == 63)
  {
    /*  interrupt_rpc_t rpc = (interrupt_rpc_t)(void*)registers->rdi;
      rpc(registers, interrupt_stack_frame,
          registers->rsi, registers->rdx, registers->rcx, registers->r8, registers->r9);
    */
  }

  // 时钟中断？
  if (vector == INT_IRQ0) // timer interrupt
  {
    Terminal::printf("tick\n");
  }

  // 对于 PIC，需要发送 EOI
  if (vector >= INT_IRQ8 && vector <= INT_IRQ15) {
    // reset slave
    IOport::outb(0xA0, 0x20);
  }
  if (vector >= INT_IRQ0 && vector <= INT_IRQ15) {
    // reset master
    IOport::outb(0x20, 0x20);
  }
}