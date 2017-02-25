#include "kernel/printk.h"
#include "kernel/irq.h"
#include "kernel/timer.h"
#include "kernel/alloc.h"
#include "kernel/memory.h"
#include "kernel/defs.h"
#include "sched/cpu.h"
#include "sched/sched.h"

void load_sections()
{
    kmemcpy(&_data_vma, &_data_lma, (u32)&_data_sz);
    kmemcpy(&_rodata_vma, &_rodata_lma, (u32)&_rodata_sz);
    for (u32 i = 0; i < (u32)&_bss_sz; i++) {
        *(u8*)(&_bss_vma + i) = 0;
    }
}

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

void taskb()
{
    u32 last_tick = 0;
    int cnt = 0;
    for (;;) {
        if (c_tick != last_tick) {
            printk("B %d\n", cnt--);
            last_tick = c_tick;
        }
    }
}

void kmain(void)
{
    load_sections();
    mm_init();

    sched_start_task(&taska);
    sched_start_task(&taskb);

    timer_init();
    sched_start();

    while (1) {
    }
}
