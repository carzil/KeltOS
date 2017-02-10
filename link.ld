ENTRY(_start)

MEMORY {
    FLASH (rx) : ORIGIN = 0x00000000, LENGTH = 0x40000
    SRAM (rwx) : ORIGIN = 0x20000000, LENGTH = 0x1ffff
}

SECTIONS {
    . = ORIGIN(FLASH);
    
    .text : {
        start_asm.o(.text);
        *(.text);
    }

    .rodata : {
        *(.rodata);
    }

    . = ORIGIN(SRAM);

    .data : {
        *(.data)
        *(.bss)
    }

    _edata = .;
}
