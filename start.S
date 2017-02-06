interrupt_vector_table:
    b _irq_handler @ reset
    b _irq_handler @ undefined instruction
    b _irq_handler @ swi
    b _irq_handler @ prefetch abort
    b _irq_handler @ data abort
    b _irq_handler @ irq
    b _irq_handler @ fiq (fast irq)

.comm stack, 0x10000

.globl _start
_start:
    ldr     sp, =stack+0x10000
    bl      main

_hang:
    b _hang
