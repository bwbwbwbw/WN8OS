#ifndef _INTERRUPT_H_
#define _INTERRUPT_H_

#include <terminal.h>
#include <runtime/types.h>
#include <cpu.h>

typedef u8 interrupt_vector_t;

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