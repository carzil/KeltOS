#include "kernel/defs_asm.h"

/* PendSV is used by KeltOS for context switching. */
FUNCTION(pendsv_handler):
    ldr     r0, =c_task
    ldr     r0, [r0]

    SAVE_CONTEXT

    bl      sched_switch_task

    RESTORE_CONTEXT

    /* jump to task */
    mov     lr, #EXC_THREAD_PROCESS
    bx      lr

FUNCTION(sched_switch_in):
    RESTORE_CONTEXT

    /* switch to process stack */
    mov     r0, #0b11
    msr     control, r0

    /* this routine is called only when scheduler starts,
     * so we don't matter about register actual values */
    pop     {r0-r3, r12, lr}
    pop     {r0, pc}
