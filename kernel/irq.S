.macro handler name, c_handler
.globl \name
.type \name, %function
\name:
    bl      \c_handler
    /* Handler returned EXC_RETURN, pass it to processor */
    bx      r0
.endm

handler _swi_handler, swi_handler
handler _systick_handler, systick_handler
