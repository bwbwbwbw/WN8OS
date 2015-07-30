#ifndef _CPU_H_
#define _CPU_H_

#include <runtime/types.h>

#ifndef ASM

typedef struct registers_struct
{
  u64 r15, r14, r13, r12, r11, r10, r9, r8, rdi, rsi, rsp, rbp, rdx, rcx, rbx, rax;
} registers_t;

#endif
#endif
