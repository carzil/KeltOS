.globl _swi_handler
_swi_handler:
    push    {lr}
    bl      swi_handler
    pop     {lr}
    mov     pc, r14
