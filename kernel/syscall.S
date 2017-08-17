#include "kernel/defs_asm.h"

    @ Parameters are passed in r0, r1, r2, r3
FUNCTION(svc_handler):
    stmfd   sp!, {r4-r12, lr}

    @ svc instruction
    ldr     r12, [lr, #-4]

    @ clear encoded instruction, r12 now holds syscall number
    bic     r12, r12, #0xff000000

    ldr     r11, =syscall_table
    ldr     r11, [r11, r12, lsl #2]

    blx     r11

    ldmfd   sp!, {r4-r12, pc}^
