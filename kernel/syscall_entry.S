#include "kernel/defs.h"

FUNCTION(swi_handler):
    ldr     r1, =c_task
    ldr     r1, [r1]

    cmp     lr, #EXC_THREAD_MAIN
    /* we are coming from kernelspace, just restore task context */
    beq     return_to_user

    /* we are coming from userspace, save task context */
    mrs     r0, psp
    stmdb   r0!, {r4-r11}

    /* reserve space for exception context */
    sub     sp, #8 * 4

    /* r0 points to full task context
     * r1 points to temporary kernel exception context */
    mov     r1, sp
    bl      __syscall_enter

    cmp     r0, #0
    /* __syscall_trampoline requested immediate return */
    bne     return_to_user

    mov     lr, #EXC_THREAD_MAIN
    bx      lr

return_to_user:
    /* __syscall_exit returns c_task */
    bl      __syscall_exit

    /* load c_task->sp */
    ldr     r1, [r0]

    /* restore context */
    ldmia   r1!, {r4-r11}
    str     r1, [r0]
    msr     psp, r1

    mov     lr, #EXC_THREAD_PROCESS
    bx      lr
