#include "kernel/defs_asm.h"

    .syntax unified
    .text
int_vec:
    b       .           @ reset handler
    b       .           @ undefined instruction
    b       svc_handler
    b       .           @ prefetch abort
    b       .           @ data abort
    b       .           @ not used
    b       _irq_handler
    b       .           @ fiq handler

.comm stack, STACK_SIZE
.comm irq_stack, IRQ_STACK_SIZE

FUNCTION(_start):
    ldr     sp, =stack+STACK_SIZE
    bl      kmain
