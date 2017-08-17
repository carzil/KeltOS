ENTRY(_start)

SECTIONS {
    .text 0x0 :
    {
        start_asm.o(.text);
        *(.text*);
    }

    .rodata ALIGN(ADDR(.text) + SIZEOF(.text), 4) :
        AT(ALIGN(LOADADDR(.text) + SIZEOF(.text), 4))
    {
        *(.rodata*);
    }

    .data ALIGN(ADDR(.rodata) + SIZEOF(.rodata), 4) : 
        AT(ALIGN(LOADADDR(.rodata) + SIZEOF(.rodata), 4))
    {
        *(.data*);
    }

    .bss ALIGN(ADDR(.data) + SIZEOF(.data), 4) :
        AT(ALIGN(LOADADDR(.data) + SIZEOF(.data), 4))
    {
        *(.bss*);
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
