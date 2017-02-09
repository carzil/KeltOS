.syntax unified
.text
int_vec:
    .word stack+0x1000 @ intial sp value
    .word _start        @ reset address
    .word 0             @ NMI
    .word 0             @ Hard Faul
    .word 0             @ Memory Fault
    .word 0             @ Bus Fault
    .word 0             @ Usage Fault
    .word 0             @ 7
    .word 0             @ To
    .word 0             @ 10 
    .word 0             @ Reserved
    .word _swi_handler  @ SVC_Handler
    .word 0             @ DebugMon_Handler
    .word 0             @ Reserved
    .word 0             @ PendSV_Handler
    .word 0             @ SysTick_Handler
    .word 0             @ IRQ_Handler
    .word 0             @ IRQ_Handler

.globl _start
.type _start, %function
_start:
    b      kmain

_hang:
    b _hang

.comm stack, 0x1000
