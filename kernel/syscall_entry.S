#include "kernel/defs.h"

FUNCTION(swi_handler):
    ldr     r0, =c_task
    ldr     r0, [r0]

    SAVE_CONTEXT

    /* SAVE_CONTEXT returns new sp in r1, it points to struct sys_regs */
    mov     r0, r1
    bl      __syscall_trampoline

    /* c_task can be switched after __syscall_trampoline */
    ldr     r0, =c_task
    ldr     r0, [r0]

    /* restore context of current task anyway */
    RESTORE_CONTEXT

    mov     lr, #EXC_THREAD_PROCESS
    bx      lr
