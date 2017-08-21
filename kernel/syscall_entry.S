#include "kernel/defs.h"

FUNCTION(swi_handler):
    ldr     r0, =c_task
    ldr     r0, [r0]

    SAVE_CONTEXT

    push    {lr}
    mrs     r0, psp
    /* get sys_number: it is stored in r0 which saved by hardware */
    ldr     r2, [r0, #0]
    ldr     r1, =syscall_table
    /* load syscall handler address */
    ldr     r3, [r1, r2, lsl #2]
    blx     r3

    pop     {lr}
    bx      lr
