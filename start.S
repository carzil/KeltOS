#include "kernel/defs.h"

.syntax unified
.text
int_vec:
    .word STACK_END-0x4 @ intial sp value
    .word kmain         @ reset address
    .word 0             @ NMI
    .word 0             @ Hard Fault
    .word 0             @ Memory Fault
    .word 0             @ Bus Fault
    .word 0             @ Usage Fault
    .word 0             @ 7
    .word 0             @ To
    .word 0             @ 10 
    .word 0             @ Reserved
    .word _swi_handler
    .word 0             @ DebugMon_Handler
    .word 0             @ Reserved
    .word 0             @ PendSV_Handler
    .word _systick_handler
    .word 0             @ IRQ_Handler
    .word 0             @ IRQ_Handler
