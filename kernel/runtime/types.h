#ifndef _TYPES_H_
#define _TYPES_H_

#ifndef ASM

// 遵循 LP64
// See more at: http://wiki.osdev.org/X86-64

// 基本单位

typedef unsigned char   u8;
typedef unsigned short  u16;
typedef unsigned int    u32;
typedef unsigned long   u64;

typedef signed char     s8;
typedef signed short    s16;
typedef signed int      s32;
typedef signed long     s64;

typedef s8 int8_t;
typedef u8 uint8_t;

typedef s16 int16_t;
typedef u16 uint16_t;

typedef s32 int32_t;
typedef u32 uint32_t;

typedef s64 int64_t;
typedef u64 uint64_t;

typedef s64 intptr_t;
typedef u64 uintptr_t;

typedef u64 size_t;

#endif

#define NULL 0;

#endif
