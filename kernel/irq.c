#include "kernel/irq.h"
#include "kernel/printk.h"
#include "sched/cpu.h"

void set_exception_handler(unsigned int exc, void* addr)
{
    // branch instruction in ARM is 24-bit relative to current PC
    *(unsigned int*)(exc) = 0xea000000 | ((((unsigned int)addr) - exc - 8) >> 2);
}

void swi_handler()
{
    printu32(__get_cpsr());
    putc('\n');
}
