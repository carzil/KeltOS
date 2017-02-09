ENTRY(_start)

MEMORY {
    FLASH (rx) : ORIGIN = 0x00000000, LENGTH = 0x10000
    SRAM (rwx) : ORIGIN = 0x20000000, LENGTH = 0x10000
}

SECTIONS {
    .text : {
        *(.text);
    } > FLASH

    .rodata : {
        *(.rodata);
    } > FLASH
    
    .data : {
        *(.data)
        *(.bss)
    } > SRAM
}
