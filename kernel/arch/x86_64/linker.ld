OUTPUT_FORMAT("elf64-x86-64")
OUTPUT_ARCH(i386:x86-64)
ENTRY (_START)

kernel_VMA = 0xffff800000000000;
kernel_LMA = 0x100000;

SECTIONS
{
    /** 链接到低地址 */
    . = kernel_LMA;
    KERNEL_START_PHYSICAL = .;

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
        TEXT_START = .;
        *(.text)
        *(.text.*)
        TEXT_END = .;
    }

    .rodata ALIGN(4K) : AT(ADDR(.rodata) - kernel_VMA)
    {
        RODATA_START = .;
        *(.rodata)
        *(.rodata.*)
        RODATA_END = .;
    }

    .data ALIGN(4K) : AT(ADDR(.data) - kernel_VMA)
    {
        DATA_START = .;
        *(.data)
        *(.data.*)
        DATA_END = .;
    }

    .ctors ALIGN(4K) : AT(ADDR(.ctors) - kernel_VMA)
    {
        CTOR_START = .;
        QUAD((CTOR_END - CTOR_START) / 8 - 1)
        *(.ctors)
        CTOR_END = .;

        INIT_ARRAY_START = .;
        QUAD((INIT_ARRAY_END - INIT_ARRAY_START) / 8 - 1)
        *(.init_array)
        INIT_ARRAY_END = .;

        /* dtor omitted. */
    }

    .bss ALIGN(4K) : AT(ADDR(.bss) - kernel_VMA)
    {
        BSS_START = .;
        *(COMMON)
        *(.bss)
        *(.bss.*)
        BSS_END = .;
    }

    KERNEL_END_PHYSICAL = . - kernel_VMA;
}
