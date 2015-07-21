OUTPUT_FORMAT("elf64-x86-64")
OUTPUT_ARCH(i386:x86-64)
ENTRY (_START)

kernel_VMA = 0xffff800000000000;
kernel_LMA = 0x100000;

SECTIONS
{
    /** 链接到低地址 */
    . = kernel_LMA;

    .init :
    {
        *(.init)
    }
    .init.bss ALIGN(4K) :
    {
        *(.init.bss)
    }

    /** 链接到高地址，存放在低地址 */
    . += kernel_VMA;

    .text ALIGN(4K) : AT(ADDR(.text) - kernel_VMA)
    {
        *(.text)
        *(.text.*)
    }

    .rodata ALIGN(4K) : AT(ADDR(.rodata) - kernel_VMA)
    {
        *(.rodata)
        *(.rodata.*)
    }

    .data ALIGN(4K) : AT(ADDR(.data) - kernel_VMA)
    {
        *(.data)
        *(.data.*)
    }

    .bss ALIGN(4K) : AT(ADDR(.bss) - kernel_VMA)
    {
        *(COMMON)
        *(.bss)
        *(.bss.*)
    }
}
