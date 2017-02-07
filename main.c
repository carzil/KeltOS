#include "kernel/printk.h"
#include "kernel/irq.h"
#include "sched/cpu.h"

int kmain(void)
{
    set_exception_handler(EXC_SWINT, &_swi_handler);
    asm ("swi #0");
    asm ("swi #0");
    asm ("swi #0");
    return 0;
}
