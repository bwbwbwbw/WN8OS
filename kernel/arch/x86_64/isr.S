/**
 * 中断处理，该代码将生成 0x00 ~ INTERRUPT_MAX(64) - 1 的中断处理函数
 * 这些函数最终转到一个通用中断处理函数处理
 */

#define ASM 1

#include <interrupt.h>

# in interrupt.h
.extern interrupt_handler

    /* ISR Handler */
    .macro GENERATE_ISR_HANDLER n
    .if \n < INTERRUPT_MAX
    .align INTERRUPT_ISR_ALIGN
    ISR_\n:
        cli
        /* 这些中断没有错误号码，因此需要手工推入一个错误码维持参数个数 */
        .if \n <= 7 || \n == 9 || \n == 16 || \n >= 18
        pushq   $0
        .endif
        pushq   $\n
        jmp     ISR_HANDLER
    .endif
    .endm

.section .text
    .code64

    /* 生成 0x00 ~ INTERRUPT_MAX 的中断处理函数 */
    .global ISR_HANDLERS
    ISR_HANDLERS:
        .irpc x, 0123456789abcdef
        .irpc y, 0123456789abcdef
        GENERATE_ISR_HANDLER 0x\x\y
        .endr
        .endr

    /* 通用处理函数，保存寄存器后会调用 interrupt_handler() */
    .align 16
    ISR_HANDLER:
        pushq   %rax
        movq    %rsp, %rax
        add     $8, %rax
        pushq   %rbx
        pushq   %rcx
        pushq   %rdx
        pushq   %rbp
        pushq   %rax # rsp
        pushq   %rsi
        pushq   %rdi

        pushq   %r8
        pushq   %r9
        pushq   %r10
        pushq   %r11
        pushq   %r12
        pushq   %r13
        pushq   %r14
        pushq   %r15

        movq    0(%rax), %rdi   # interrupt vector number
        movq    8(%rax), %rsi   # interrupt error code
        movq    %rsp, %rdx      # general purpose registers
        movq    %rax, %rcx      # interrupt stack frame
        add     $16, %rcx

        call    interrupt_handler

        popq    %r15
        popq    %r14
        popq    %r13
        popq    %r12
        popq    %r11
        popq    %r10
        popq    %r9
        popq    %r8

        popq    %rdi
        popq    %rsi
        add     $8, %rsp # rsp
        popq    %rbp
        popq    %rdx
        popq    %rcx
        popq    %rbx
        popq    %rax

        /* 弹出中断向量和错误码 */
        add     $16, %rsp

        sti
        iretq