.globl _swi_handler
.type _swi_handler, %function
_swi_handler:
    bl       swi_handler
    bx       r0

