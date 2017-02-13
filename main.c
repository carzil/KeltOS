#include "kernel/printk.h"
#include "kernel/irq.h"
#include "kernel/timer.h"
#include "kernel/alloc.h"
#include "kernel/memory.h"
#include "kernel/defs.h"
#include "sched/cpu.h"

void load_sections()
{
    kmemcpy(&_data_vma, &_data_lma, (u32)&_data_sz);
    kmemcpy(&_rodata_vma, &_rodata_lma, (u32)&_rodata_sz);
    for (u32 i = 0; i < (u32)&_bss_sz; i++) {
        *(u8*)(&_bss_vma + i) = 0;
    }
    data_brk = &_data_brk;
}

void kmain(void)
{
    load_sections();
    timer_init();
    mm_init();

    while (1) {
    }
}
