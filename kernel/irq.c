#include "kernel/irq.h"
#include "kernel/printk.h"
#include "sched/cpu.h"

int swi_handler()
{
    putc('s');
    return 0xFFFFFFF1;
}
