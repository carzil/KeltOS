    .syntax unified

/* PendSV is used by KeltOS for context switching. */

    .globl pendsv_handler
    .type pendsv_handler, %function
pendsv_handler:
    ldr     r0, =c_task
    ldr     r0, [r0]

    /* processor already saved r0, r1, r2, r3, r12,
       pc, lr and sp, save the rest */
    mrs     r1, psp
    stmdb   r1!, {r4-r11}
    str     r1, [r0, #0]

    bl      sched_switch_task

    /* r0 already cotains *c_task */
    ldr     r1, [r0, #0]

    /* restore context */
    ldmia   r1!, {r4-r11}
    msr     psp, r1
     
    /* jump to task */
    mov     lr, #0xfffffffd
    bx      lr

    .global sys_sched_start
    .type sys_sched_start, %function
sys_sched_start:
    /* enable scheduler */
    ldr     r0, =sched_enabled
    mov     r1, #1
    str     r1, [r0]

    bl      sched_switch_task

    /* get stack top */
    ldr     r1, [r0, #0]

    /* restore context */
    ldmia   r1!, {r4-r11}

    /* save new stack pointer after restore */
    str     r1, [r0, #0]
    msr     psp, r1

    /* switch to thread mode, using process stack */
    mov     lr, #0xfffffffd
    bx      lr
