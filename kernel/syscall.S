    .globl swi_handler
    .type swi_handler, %function
swi_handler:
    mrs     r0, psp
    bl      syscall_handler
