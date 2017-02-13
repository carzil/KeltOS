#include "kernel/irq.h"
#include "kernel/printk.h"
#include "sched/cpu.h"

u32 swi_handler()
{
    return 0xFFFFFFF1;
}
