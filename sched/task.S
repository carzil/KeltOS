#include "kernel/defs.h"

    /* PendSV is used by KeltOS for context switching. */
FUNCTION(pendsv_handler):
    ldr     r0, =c_task
    ldr     r0, [r0]

    /* save context of old task */
    SAVE_CONTEXT

    bl      sched_switch_task

    /* restore context of new task */
    RESTORE_CONTEXT

    /* return into new task */
    mov     lr, #EXC_THREAD_PROCESS
    bx      lr
