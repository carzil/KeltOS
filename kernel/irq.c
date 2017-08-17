#include "kernel/irq.h"
#include "kernel/printk.h"
#include "sched/cpu.h"
#include "kernel/types.h"
#include "drivers/timer.h"
#include "kernel/timer.h"
#include "drivers/vic.h"
#include "sched/sched.h"


void irq_handler()
{
    u32 intp;
    vic_for_each_active(intp) {
        if (intp == TIMER_IRQ) {
            timer_handler();
            timer_clear_interrupt(timer0);
        }
    }

    if (c_task->need_reschedule) {
        c_task->need_reschedule = 0;
        struct task* new_task = sched_switch_task();
        sched_switch_in(new_task);
    }
}
