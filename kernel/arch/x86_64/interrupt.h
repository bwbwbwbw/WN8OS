#ifndef _INTERRUPT_H_
#define _INTERRUPT_H_

#include <runtime/types.h>
#include <cpu.h>

#define INTERRUPT_MAX         64
#define INTERRUPT_ISR_ALIGN   16

#ifndef ASM

namespace interrupt
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

  typedef struct idt_ptr_struct
  {
    u16 limit;
    u64 base;
  } __attribute__((packed)) idt_ptr_t;

  typedef struct idt_entry_struct
  {
    u16 base_lo;
    u16 select;
    u8 zero_1;
    u8 flags;
    u16 base_mi;
    u32 base_hi;
    u32 zero_2;
  } __attribute__((packed)) idt_entry_t;

  typedef struct interrupt_stack_frame_struct
  {
    u64 rip, cs, rflags, rsp, ss;
  } interrupt_stack_frame_t;

  typedef void (*interrupt_handler_t) (
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
#endif
