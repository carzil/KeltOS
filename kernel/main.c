#include "kernel/printk.h"
#include "kernel/irq.h"
#include "kernel/timer.h"
#include "kernel/alloc.h"
#include "kernel/memory.h"
#include "kernel/defs.h"
#include "kernel/spinlock.h"
#include "kernel/bug.h"
#include "sched/cpu.h"
#include "sched/sched.h"
#include "drivers/systick.h"
#include "drivers/semihosting.h"
#include "reactor/reactor.h"

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
    asm (
        "mov r0, #700\n"
        "mov r1, #701\n"
        "mov r2, #702\n"
        "mov r3, #703\n"
        "mov r4, #704\n"
        "mov r5, #705\n"
        "mov r6, #706\n"
        "mov r7, #707\n"
        "mov r8, #708\n"
        "mov r9, #709\n"
        "mov r10, #710\n"
        "mov r11, #711\n"
        "mov r12, #712\n"
    );
    for (;;) {
        asm (
            "mov r0, #1\n"
            "swi #0"
        );
        // if (c_tick != last_tick) {
        //     printk("A %d\n", cnt++);
        //     last_tick = c_tick;
        // }
    }
}

void taskd(struct reactor_event* ev)
{
    // u32 last_tick = 0;
    // int cnt = 0;
    smhost_printz("test event delivered\n");
    asm (
        "mov    r0, #2\n"
        "svc    #0"
    );
        // asm (
        //     "mov r0, #1\n"
        //     "swi #0"
        // );
        // if (c_tick != last_tick) {
        //     // smhost_printz("task d\n");
        //     printk("DDD %d\n", cnt++);
        //     last_tick = c_tick;
        //     cnt++;
        // }
}

void kmain(void)
{
    irq_disable_force();
    load_sections();
    mm_init();
    sched_init();
    reactor_init();
    s32 id = reactor_register_event_type("org.kelt.TestEvent");
    timer_init();
    systick_init();
    printk_init();

    struct task* tsk = sched_create_task(PRIORITY_NORMAL);
    tsk->name = "task D";
    reactor_watch_for(tsk, id, &taskd);

    /* prepare scheduler */
    sched_start();

    /* reset MSP */
    set_msp(STACK_END);

    /* set pendsv pending and enable irqs, isb is used to wait for PendSV */
    sched_context_switch();
    irq_enable_force();
    isb();

    BUG_ON_REACH();
}
