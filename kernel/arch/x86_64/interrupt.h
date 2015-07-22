#ifndef _INTERRUPT_H_
#define _INTERRUPT_H_

#include <ioport.h>
#include <terminal.h>
#include <runtime/types.h>
#include <cpu.h>

typedef u8 interrupt_vector_t;

#define INT_IRQ0            ((interrupt_vector_t)0x20)
#define INT_IRQ1            (INT_IRQ0 + 1)
#define INT_IRQ2            (INT_IRQ0 + 2)
#define INT_IRQ3            (INT_IRQ0 + 3)
#define INT_IRQ4            (INT_IRQ0 + 4)
#define INT_IRQ5            (INT_IRQ0 + 5)
#define INT_IRQ6            (INT_IRQ0 + 6)
#define INT_IRQ7            (INT_IRQ0 + 7)
#define INT_IRQ8            ((interrupt_vector_t)0x28)
#define INT_IRQ9            (INT_IRQ8 + 1)
#define INT_IRQ10           (INT_IRQ8 + 2)
#define INT_IRQ11           (INT_IRQ8 + 3)
#define INT_IRQ12           (INT_IRQ8 + 4)
#define INT_IRQ13           (INT_IRQ8 + 5)
#define INT_IRQ14           (INT_IRQ8 + 6)
#define INT_IRQ15           (INT_IRQ8 + 7)

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

#endif