#include "kernel/defs.h"

    .syntax unified
    .section ".int_vec", "a"
int_vec:
    .word STACK_END-0x4
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

FUNCTION(nmi_handler):
    b .

FUNCTION(hard_fault_handler):
    b .

FUNCTION(memory_fault_handler):
    b .

FUNCTION(bus_fault_handler):
    b .

FUNCTION(usage_fault_handler):
    b .

FUNCTION(debugmon_handler):
    b .
