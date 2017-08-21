ENTRY(kmain)

MEMORY
{
    FLASH (rx) : ORIGIN = 0x00000000, LENGTH = 1m
    SRAM (rwx) : ORIGIN = 0x20000000, LENGTH = 128k
}

SECTIONS {
    .text :
    {
        *(.int_vec);
        *(.text*);
        *(.rodata);
    } >FLASH

    .data :
    {
        *(.data*);
    } >SRAM AT>FLASH

    .bss :
    {
        *(.bss*);
    } >SRAM

    _data_load = LOADADDR(.data);
    _data_start = ADDR(.data);
    _data_end = _data_start + SIZEOF(.data);

    _bss_start = ADDR(.bss);
    _bss_end = _bss_start + SIZEOF(.bss);

    _data_brk = _bss_end;
}
