#ifndef _INTERRUPT_H_
#define _INTERRUPT_H_

#include <ioport.h>
#include <terminal.h>
#include <runtime/types.h>
#include <cpu.h>

namespace Interrupt
{

  typedef u8 interrupt_vector_t;

  static const interrupt_vector_t INT_IRQ0  = ((interrupt_vector_t)0x20);
  static const interrupt_vector_t INT_IRQ1  = (INT_IRQ0 + 1);
  static const interrupt_vector_t INT_IRQ2  = (INT_IRQ0 + 2);
  static const interrupt_vector_t INT_IRQ3  = (INT_IRQ0 + 3);
  static const interrupt_vector_t INT_IRQ4  = (INT_IRQ0 + 4);
  static const interrupt_vector_t INT_IRQ5  = (INT_IRQ0 + 5);
  static const interrupt_vector_t INT_IRQ6  = (INT_IRQ0 + 6);
  static const interrupt_vector_t INT_IRQ7  = (INT_IRQ0 + 7);
  static const interrupt_vector_t INT_IRQ8  = ((interrupt_vector_t)0x28);
  static const interrupt_vector_t INT_IRQ9  = (INT_IRQ8 + 1);
  static const interrupt_vector_t INT_IRQ10 = (INT_IRQ8 + 2);
  static const interrupt_vector_t INT_IRQ11 = (INT_IRQ8 + 3);
  static const interrupt_vector_t INT_IRQ12 = (INT_IRQ8 + 4);
  static const interrupt_vector_t INT_IRQ13 = (INT_IRQ8 + 5);
  static const interrupt_vector_t INT_IRQ14 = (INT_IRQ8 + 6);
  static const interrupt_vector_t INT_IRQ15 = (INT_IRQ8 + 7);

  struct interrupt_stack_frame_struct
  {
    u64 rip, cs, rflags, rsp, ss;
  };
  typedef struct interrupt_stack_frame_struct interrupt_stack_frame_t;

  typedef u64 (*interrupt_handler_t)
  (
    interrupt_vector_t vector,
    u64 error_code,
    registers_t *registers,
    interrupt_stack_frame_t *interrupt_stack_frame
  );

  typedef bool interrupt_state_t;

  void enable();
  void disable();
  void eoi_master();
  void eoi_slave();
  void remap_pic();
  void irq_mask(u16 mask);
  void init_timer(u32 frequency);

}

#endif