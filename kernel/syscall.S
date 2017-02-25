    .globl swi_handler
    .type swi_handler, %function
swi_handler:
    mov     r0, sp
    bl      syscall_handler
