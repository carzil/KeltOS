#include "kernel/printk.h"
#include "kernel/irq.h"
#include "kernel/alloc.h"
#include "sched/cpu.h"

int kmain(void)
{
    // u32* s = kalloc(10);
    puts("HAHAHAHHA\n");
    printk("Answer is %u.\nHa-ha. Just kidding. It's %u\n", 42, 4294967295);
    printk("Let's visit %x, %x.\n", 0xcafe, 0xbabe);
    printk("%d + %d = %d", -2147483647, 2147483647, -2147483647 + 2147483647);
    return 0;
}
