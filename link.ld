ENTRY(kmain)

MEMORY
{
    FLASH (rx) : ORIGIN = 0x00000000, LENGTH = 1m
    SRAM (rwx) : ORIGIN = 0x20000000, LENGTH = 128k
}

SECTIONS {
    .text :
    {
        start_asm.o(.text);
        *(.text*);
        *(.rodata);
    } >FLASH

    .data :
    {
        _data_load = LOADADDR(.data);
        _data_start = .;
        *(.data*);
        _data_end = .;
    } >SRAM AT>FLASH

    .bss :
    {
        _bss_start = .;
        *(.bss*);
        _bss_end = .;
    } >SRAM

    _data_brk = _bss_end;
}
