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
    for (;;) {
        if (c_tick != last_tick) {
            // smhost_printz("task d\n");
            printk("DDD %d\n", cnt++);
            last_tick = c_tick;
            cnt++;
        }
    }
}

void kmain(void)
{
    irq_disable();
    load_sections();
    mm_init();
    systick_init();
    printk_init();

    struct task* tsk = sched_start_task(&taskd, PRIORITY_HIGH);
    tsk->name = "task D";
    struct task* tsk2 = sched_start_task(&taska, PRIORITY_HIGH);
    tsk2->name = "task A";

    irq_enable();
    sched_start();
    isb();
    for (;;);
}
