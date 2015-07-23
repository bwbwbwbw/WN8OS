/**
 * 启动内核，GRUB < 2 只能识别 32 位内核，这部分代码将完成 32 位到 64 位的切换，以及低地址到高地址的切换
 */

#define ASM 1
#include "multiboot.h"
#include "arch.h"

/**
 * 低地址区域
 */
.section .init,"a"

    /**
     * multiboot
     */
    .align 4
    MULTIBOOT:
        /* 0x100000 */
        /* magic */
        .long   MULTIBOOT_HEADER_MAGIC
        /* flag */
        .long   MULTIBOOT_HEADER_FLAGS
        /* checksum */
        .long   -(MULTIBOOT_HEADER_MAGIC + MULTIBOOT_HEADER_FLAGS)

    /**
     * 入口
     */
    .align 4
    .global _START
    _START:
    START32:
    .code32
        /* 为了让某些反汇编器能正常反汇编之后的代码..这里加几个 nop */
        nop
        nop
        nop
        nop

        /* 0x10000C */
        /*
           保存 GRUB 传的参放以后再用
           EAX = MULTIBOOT_BOOTLOADER_MAGIC
           EBX = &(multiboot_structure)
         */
        movl    %ebx, %esi
        movl    %eax, %edi
        
        /* 关闭中断 */
        cli

        /* CR4.PAE = 1 */
        movl    %cr4, %eax
        bts     $5, %eax
        movl    %eax, %cr4

        /* CR3 = &PML4 */
        movl    $PML4_BASE, %eax
        movl    %eax, %cr3

        /* 开启长模式 */
        movl    $0xC0000080, %ecx
        rdmsr
        bts     $8, %eax
        bts     $0, %eax
        wrmsr

        /* GDT */
        movl    $(GDT_32), %eax
        lgdt    (%eax)

        /* 开启分页 */
        movl    %cr0, %eax
        bts     $31, %eax
        movl    %eax, %cr0

        /* 跳转到 64 位代码 */
        ljmp    $CS_KERNEL, $(START64)

    START64:
    .code64
        /* 重置段寄存器 */
        movw    $DS_KERNEL, %bx
        movw    %bx, %ds
        movw    %bx, %es
        movw    %bx, %fs
        movw    %bx, %gs

        movw    $0, %bx
        movw    %bx, %ss

        /* 初始化64位的栈 */
        movq    $(STACK_TOP - KERNEL_VMA_BASE), %rsp

        /* 往栈里堆高地址，利用 ret 跳转 */
        push    $CS_KERNEL
        movq    $(KERNEL_VMA_BASE >> 32), %rax
        shl     $32, %rax
        or      $(LONG_ENTRY - (KERNEL_VMA_BASE & 0xffffffff00000000)), %rax
        push    %rax

        /* 进入高地址 */
        ret

/**
 * 低地址 .bss 区域
 */
.section .init.bss,"aw"

    /**
     * GDT
     */
    .align 4096
    GDT_32:
        .hword GDT_END - GDT_DATA - 1
        .long GDT_DATA

    .align 4096
    GDT_DATA:
        /* null */
        .quad 0x0000000000000000

        /* flag=1100, base=0x0, limit=0xFFFFF, access=10011010, 32 位内核代码段: CS_KERNEL32 */
        .quad 0x00cf9a000000ffff

        /* flag=1010, base=0x0, limit=0xFFFFF, access=10011010, 64 位内核代码段: CS_KERNEL */
        .quad 0x00af9a000000ffff,0

        /* flag=1010, base=0x0, limit=0xFFFFF, access=10010011, 64 位内核数据段: DS_KERNEL */
        .quad 0x00af93000000ffff,0

        /* flag=1010, base=0x0, limit=0xFFFFF, access=11111010, 64 位用户代码段: CS_USER */
        .quad 0x00affa000000ffff,0

        /* flag=1010, base=0x0, limit=0xFFFFF, access=11110011, 64 位用户数据段: DS_USER */
        .quad 0x00aff3000000ffff,0
        .quad 0,0
        .quad 0,0
        .quad 0,0
        .quad 0,0

        /* TLS */
        .quad 0,0,0
        .quad 0x0000f40000000000
    GDT_END:

    /**
     * 临时页表
     * CR3 -> PML4 -> PML3 -> PML2 -> PML1 -> ADDR
     */
    .align 4096
    PML4_BASE:
        # 映射线性地址低半区最低一块区域和高半区最低一块区域（entry 0x0，entry 0x256）
        # 注意 PML3, PML2, PML1 已经 4K 对齐了，因此最低 12 位都是 0
        .quad (PML3_BASE + 0x7)
        .rept 255
        .quad 0
        .endr
        .quad (PML3_BASE + 0x7)
        .rept 255
        .quad 0
        .endr

    .align 4096
    PML3_BASE:
        # Page Direcotory Pointer Table 映射最低一项
        .quad (PML2_BASE + 0x7)
        .rept 511
        .quad 0
        .endr

    .align 4096
    PML2_BASE:
        # Page Directory Table 映射最低 25 项到各个 PML1 表（50MB）
        .set i, 0
        .rept 25
        .quad (PML1_BASE + i + 0x7)
        .set i, i + 4096
        .endr
        .rept (512-25)
        .quad 0
        .endr

    .align 4096
    PML1_BASE:
        # Page Table 每项 4KB，最低 12 位是 flag
        # 一共 25 个 Page Table，每个 Page Table 包含 512 项（2MB）
        .set i, 0
        .rept 512*25
        .quad (i << 12) | 0x087
        .set i, i + 1
        .endr

/**
 * 高地址区域
 */
.section .data,"aw"
    
    /**
     * 高地址 64 位代码
     */
    .align 4
    LONG_ENTRY:
    .code64
        /* 建立高地址 64 位栈 */
        movq    $(KERNEL_VMA_BASE >> 32), %rax
        shl     $32, %rax
        or      $(STACK_BOTTOM - (KERNEL_VMA_BASE & 0xffffffff00000000)), %rax
        movq    %rax, %rsp

        /* 修改 IO 权限 */
        pushf
        pop     %rax
        or      $0x3000, %rax
        push    %rax
        popf

        /* 更新 multiboot 结构体指向虚拟地址 */
        add     $(KERNEL_VMA_BASE & 0xffffffff), %rsi

        /* 传递 GRUB 参数 */
        push    %rsi
        push    %rdi

        call    kernel_main

        /* infinite loop */
    HALT:
        jmp     HALT

/**
 * 高地址区域
 */
.section .bss,"aw",@nobits
    
    /**
     * 栈
     */
    .align 4096
    STACK_BOTTOM:
    .skip STACK_SIZE
    STACK_TOP:
