#include "kernel/defs.h"

    .section ".int_vec", "a"
int_vec:
    .word stack_end
    .word kmain
    .word nmi_handler
    .word hard_fault_handler
    .word memory_fault_handler
    .word bus_fault_handler
    .word usage_fault_handler
    .word 0
    .word 0
    .word 0
    .word 0
    .word swi_handler
    .word debugmon_handler
    .word 0
    .word pendsv_handler
    .word systick_handler

    /* here comes interrupts vector */

    .section ".startup_stack", "a"
stack:
    .zero STACK_SIZE
stack_end:


ISR_HANDLER(nmi_handler):
    b .

ISR_HANDLER(hard_fault_handler):
    b .

ISR_HANDLER(memory_fault_handler):
    b .

ISR_HANDLER(bus_fault_handler):
    b .

ISR_HANDLER(usage_fault_handler):
    b .

ISR_HANDLER(debugmon_handler):
    b .
