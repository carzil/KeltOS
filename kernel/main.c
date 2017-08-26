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
#include "kernel/reactor.h"

void load_sections()
{
    kmemcpy(&_data_start, &_data_load, (u32)&_data_end - (u32)&_data_start);
    kmemset(&_bss_start, '\0', (u32)&_bss_end - (u32)&_bss_start);
}

void test_task(const char* message)
{
    smhost_printz(message);
    asm (
        "mov    r0, #3\n"
        "svc    #0"
    );
    asm (
        "mov    r0, #2\n"
        "svc    #0"
    );
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
    tsk->name = "test task";
    reactor_watch_for(tsk, id, &test_task);

    /* start scheduling */
    sched_start();

    BUG_ON_REACH();
}
