.globl _irq_handler
_irq_handler:
    mov r0, #'c'
    bl putc
