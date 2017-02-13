ENTRY(kmain)

MEMORY
{
    FLASH (rx) : ORIGIN = 0x00000000, LENGTH = 256k
    SRAM (rwx) : ORIGIN = 0x20000000, LENGTH = 128k
}

SECTIONS {
    .text ORIGIN(FLASH) :
    {
        start_asm.o(.text);
        *(.text);
    }

    .rodata ORIGIN(SRAM) :
        AT(ALIGN(LOADADDR(.text) + SIZEOF(.text), 4))
    {
        *(.rodata);
    }

    .data ALIGN(ADDR(.rodata) + SIZEOF(.rodata), 4) : 
        AT(ALIGN(LOADADDR(.rodata) + SIZEOF(.rodata), 4))
    {
        *(.data)
    }

    .bss ALIGN(ADDR(.data) + SIZEOF(.data), 4) :
        AT(ALIGN(LOADADDR(.data) + SIZEOF(.data), 4))
    {
        *(.bss)
    }

    _rodata_lma = LOADADDR(.rodata);
    _rodata_vma = ADDR(.rodata);
    _rodata_sz = SIZEOF(.rodata);

    _data_lma = LOADADDR(.data);
    _data_vma = ADDR(.data);
    _data_sz = SIZEOF(.data);

    _bss_vma = ADDR(.bss);
    _bss_sz = SIZEOF(.bss);
    _data_brk = ALIGN(ADDR(.bss) + SIZEOF(.bss), 4);
}
