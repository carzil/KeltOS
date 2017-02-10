#include "kernel/printk.h"
#include "kernel/irq.h"
#include "kernel/alloc.h"
#include "sched/cpu.h"

int kmain(void)
{
    // u32* s = kalloc(10);
    puts("HAHAHAHHA\n");
    return 0;
}
