#ifndef _ARCH_H_
#define _ARCH_H_

#include <runtime/types.h>

// kernel 位置
#define KERNEL_VMA_BASE     0xFFFF800000000000
#define KERNEL_LMA_BASE     0x100000

// GDT 项所在段
#define CS_KERNEL32         0x08      // RPL = 0, TI = 0, INDEX = 0x1 (0001 0 00)
#define CS_KERNEL           0x10      // RPL = 0, TI = 0, INDEX = 0x2 (0010 0 00)
#define DS_KERNEL           0x20      // RPL = 0, TI = 0, INDEX = 0x4 (0100 0 00)
#define CS_USER             0x33      // RPL = 3, TI = 0, INDEX = 0x6 (0110 0 11)
#define DS_USER             0x43      // RPL = 3, TI = 0, INDEX = 0x8 (1000 0 11)

#endif
