/**
 * 启动内核
 * GRUB 只能识别 32 位内核，因此这部分代码是 32 位的
 */

#define ASM 1
#include "multiboot.h"

/**
 * 入口
 */
.section .text
.global _start
.type _start, @function
.align 4
_start:
.code32
    jmp     (boot_entry)

/**
 * multiboot
 */
.section .multiboot
.align 4
    /* magic */
    .long   MULTIBOOT_HEADER_MAGIC
    /* flag */
    .long   MULTIBOOT_HEADER_FLAGS
    /* checksum */
    .long   -(MULTIBOOT_HEADER_MAGIC + MULTIBOOT_HEADER_FLAGS)

/**
 * 栈区域
 */
.section .bootstrap_stack, "aw", @nobits
stack_bottom:
.skip STACK_SIZE
stack_top:

/**
 * 主程序
 */
.section .entry
boot_entry:
.code32
    movl    $stack_top, %esp

    /* Reset EFLAGS */
    pushl   $0
    popf

    /* GRUB 传的参放入栈中 */
    pushl   %ebx
    pushl   %eax

    call    kernel_main

    /* Halt */
    cli
    hlt
loop:
    jmp     loop
