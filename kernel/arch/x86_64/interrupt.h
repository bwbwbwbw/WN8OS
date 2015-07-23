#ifndef _INTERRUPT_H_
#define _INTERRUPT_H_

#include <runtime/types.h>

#include <isr.h>
#include <ioport.h>
#include <terminal.h>
#include <cpu.h>

namespace Interrupt
{

  typedef u8 interrupt_vector_t;

  extern const interrupt_vector_t INT_IRQ0;
  extern const interrupt_vector_t INT_IRQ1;
  extern const interrupt_vector_t INT_IRQ2;
  extern const interrupt_vector_t INT_IRQ3;
  extern const interrupt_vector_t INT_IRQ4;
  extern const interrupt_vector_t INT_IRQ5;
  extern const interrupt_vector_t INT_IRQ6;
  extern const interrupt_vector_t INT_IRQ7;
  extern const interrupt_vector_t INT_IRQ8;
  extern const interrupt_vector_t INT_IRQ9;
  extern const interrupt_vector_t INT_IRQ10;
  extern const interrupt_vector_t INT_IRQ11;
  extern const interrupt_vector_t INT_IRQ12;
  extern const interrupt_vector_t INT_IRQ13;
  extern const interrupt_vector_t INT_IRQ14;
  extern const interrupt_vector_t INT_IRQ15;

  struct interrupt_stack_frame_struct
  {
    u64 rip, cs, rflags, rsp, ss;
  };
  typedef struct interrupt_stack_frame_struct interrupt_stack_frame_t;

  typedef void (*interrupt_handler_t)
  (
    interrupt_vector_t vector,
    u64 error_code,
    registers_t *registers,
    interrupt_stack_frame_t *interrupt_stack_frame
  );

  typedef bool interrupt_state_t;

  void init();
  void enable();
  void disable();
  void eoi_master();
  void eoi_slave();
  void remap_pic();
  void irq_mask(u16 mask);
  void init_timer(u32 frequency);

  void register_handler(interrupt_vector_t vector, interrupt_handler_t handler);

}

#endif