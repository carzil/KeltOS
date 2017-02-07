.comm stack, 0x10000

.globl _start

_start:
    ldr     sp, =stack+0x10000
    ldr     lr, =_hang
    ldr     pc, =kmain

_hang:
    b _hang
