#include "kernel/defs.h"

    /* PendSV is used by KeltOS for context switching. */
FUNCTION(pendsv_handler):
    ldr     r0, =c_task
    ldr     r0, [r0]

    /* get current task sp */
    cmp     lr, #EXC_THREAD_MAIN
    mrseq   r1, msp
    mrsne   r1, psp

    stmdb   r1!, {r4-r11}

    cmp     lr, #EXC_THREAD_MAIN
    msreq   msp, r1

    /* c_task->sp = r1 */
    str     r1, [r0]

    bl      __sched_switch_task

    /* sched_switch_task returned new task in r0 */
    ldr     r1, [r0]
    ldmia   r1!, {r4-r11}

    ldr     r2, [r0, #8]
    tst     r2, #TASK_SYSCALL_BIT
    msreq   psp, r1
    msrne   msp, r1
    moveq   lr, #EXC_THREAD_PROCESS
    movne   lr, #EXC_THREAD_MAIN

    /* return into new task */
    bx      lr
