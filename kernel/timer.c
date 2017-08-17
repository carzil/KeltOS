#include "kernel/timer.h"
#include "sched/sched.h"

volatile u32 c_tick = 0;

void timer_handler()
{
    c_tick++;
    if (c_tick % 100 == 0) {
        c_task->need_reschedule = 1;
    }
}
