#ifndef _CPU_H_
#define _CPU_H_

#include <runtime/types.h>

struct registers_struct
{
  u64 r15, r14, r13, r12, r11, r10, r9, r8, rdi, rsi, rsp, rbp, rdx, rcx, rbx, rax;
};
typedef struct registers_struct registers_t;

#endif