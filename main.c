#include "kernel/printk.h"
#include "kernel/irq.h"
#include "sched/cpu.h"

int kmain(void)
{
    asm ("swi #0");
    asm ("swi #0");
    asm ("swi #0");
    return 0;
}
