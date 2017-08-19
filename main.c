#include "kernel/printk.h"
#include "kernel/irq.h"
#include "kernel/timer.h"
#include "kernel/alloc.h"
#include "kernel/memory.h"
#include "kernel/defs.h"
#include "kernel/spinlock.h"
#include "sched/cpu.h"
#include "sched/sched.h"
#include "drivers/systick.h"
#include "drivers/semihosting.h"

void load_sections()
{
    kmemcpy(&_data_start, &_data_load, (u32)&_data_end - (u32)&_data_start);
    kmemset(&_bss_start, '\0', (u32)&_bss_end - (u32)&_bss_start);
}

struct spinlock spl;

void taska()
{
    u32 last_tick = 0;
    int cnt = 0;
    for (;;) {
        if (c_tick != last_tick) {
            printk("A %d\n", cnt++);
            last_tick = c_tick;
        }
    }
}

void taskd()
{
    u32 last_tick = 0;
    int cnt = 0;
    for (; cnt < 100000;) {
        if (c_tick != last_tick) {
            // smhost_printz("task d\n");
            printk("DDD %d\n", cnt++);
            last_tick = c_tick;
            cnt++;
        }
    }
    printk("Task D exit\n");
    asm (
        "mov    r0, #0\n"
        "swi    #32\n"
    );
}

void kmain(void)
{
    irq_disable();
    load_sections();
    mm_init();
    systick_init();
    printk_init();

    struct task* taska = sched_start_task(&taskd, PRIORITY_HIGH);
    taska->name = "task D";
    // sched_start_task(&taskb, PRIORITY_NORMAL);
    // sched_start_task(&taskd, PRIORITY_NORMAL);
    // sched_start_task(&taskc, PRIORITY_NORMAL);
    irq_enable();
    sched_start();
}
