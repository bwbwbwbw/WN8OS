OUTPUT_FORMAT("elf64-x86-64")
OUTPUT_ARCH(i386:x86-64)
ENTRY (_START)

kernel_VMA = 0xffff800000000000;
kernel_LMA = 0x100000;

SECTIONS
{
    /** 链接到低地址 */
    . = kernel_LMA;
    KERNEL_START = .;

    .init32 :
    {
        INIT_START = .;
        *(.init32)
        INIT_END = .;
    }
    .init32.bss ALIGN(4K) :
    {
        INIT_BSS_START = .;
        *(.init32.bss)
        INIT_BSS_END = .;
    }

    /** 链接到高地址，存放在低地址 */
    . += kernel_VMA;

    .text ALIGN(4K) : AT(ADDR(.text) - kernel_VMA)
    {
        TEXT_START = . - kernel_VMA;
        *(.text)
        *(.text.*)
        TEXT_END = . - kernel_VMA;
    }

    .rodata ALIGN(4K) : AT(ADDR(.rodata) - kernel_VMA)
    {
        RODATA_START = . - kernel_VMA;
        *(.rodata)
        *(.rodata.*)
        RODATA_END = . - kernel_VMA;
    }

    .data ALIGN(4K) : AT(ADDR(.data) - kernel_VMA)
    {
        DATA_START = . - kernel_VMA;
        *(.data)
        *(.data.*)
        DATA_END = . - kernel_VMA;
    }

    .ctors ALIGN(4K) :
    {
        __CTOR_LIST__ = .;
        QUAD((__CTOR_END__ - __CTOR_LIST__) / 8 - 1)
        *(.ctors)
        __CTOR_END__ = .;

        __INIT_ARRAY_LIST__ = .;
        QUAD((__INIT_ARRAY_END__ - __INIT_ARRAY_LIST__) / 8 - 1)
        *(.init_array)
        __INIT_ARRAY_END__ = .;

        /* dtor omitted. */
    }

    .bss ALIGN(4K) : AT(ADDR(.bss) - kernel_VMA)
    {
        BSS_START = . - kernel_VMA;
        *(COMMON)
        *(.bss)
        *(.bss.*)
        BSS_END = . - kernel_VMA;
    }

    KERNEL_END = . - kernel_VMA;
}
