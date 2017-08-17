#include "kernel/printk.h"
#include "kernel/irq.h"
#include "kernel/timer.h"
#include "kernel/alloc.h"
#include "kernel/memory.h"
#include "kernel/defs.h"
#include "kernel/spinlock.h"
#include "sched/cpu.h"
#include "sched/sched.h"
#include "drivers/semihosting.h"
#include "drivers/timer.h"
#include "drivers/vic.h"

void load_sections()
{
    kmemcpy(&_data_vma, &_data_lma, (u32)&_data_sz);
    kmemcpy(&_rodata_vma, &_rodata_lma, (u32)&_rodata_sz);
    for (u32 i = 0; i < (u32)&_bss_sz; i++) {
        *(u8*)(&_bss_vma + i) = 0;
    }
    printk("data size: %u bytes, rodata: %u bytes, bss: %u bytes\n", (u32)&_data_sz, (u32)&_rodata_sz, (u32)&_bss_sz);
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

void taskd()
{
    u32 last_tick = 0;
    int cnt = 0;
    for (; cnt < 100;) {
        if (c_tick != last_tick) {
            // smhost_printz("task d\n");
            printk("DDD %d\n", cnt++);
            last_tick = c_tick;
            cnt++;
        } else {
            asm (
                "mov    r0, #1\n"
                "svc    #1\n"
            );
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
    cpu_init();
    mm_init();
    vic_unmask_interrupt(TIMER_IRQ);

    timer_load(timer0, 320);
    timer_ctl(timer0, TIMER_ENABLE | TIMER_PERIODIC | TIMER_IRQ_ENABLE | TIMER_32_BIT);

    enable_irq();

    // void* ptr = kalloc(8192);

    asm (
        "mrs    r0, cpsr\n"
        "and    r0, #0xffffffe0\n"
        "orr    r0, #0b10000\n"
        "msr    cpsr, r0\n"
    );

    for (;;) {
    }
}
