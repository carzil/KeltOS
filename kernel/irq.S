#include "kernel/defs_asm.h"

FUNCTION(_irq_handler):
    @ fix lr instruction
    sub     lr, lr, #4

    @ save r0_usr and lr_svc
    stmia   sp, {r0, lr}
    mrs     r0, spsr

    @ save cpsr_usr
    str     r0, [sp, #8]
    ldr     r0, =c_task
    ldr     r0, [r0]

    @ switch mode
    mrs     r0, cpsr
    eor     r0, #IRQ_MODE ^ SVC_MODE
    msr     cpsr, r0

    ldr     r0, =c_task
    ldr     r0, [r0]
    ldr     sp, [r0,  #4]
