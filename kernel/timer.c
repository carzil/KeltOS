#include "kernel/timer.h"
#include "kernel/irq.h"
#include "sched/sched.h"
#include "drivers/semihosting.h"

volatile u32 c_tick = 0;

void timer_tick()
{
    irq_disable_safe(tmp);
    c_tick++;
    if (sched_enabled && (c_tick % 100) == 0) {
        sched_context_switch();
    }
    irq_enable_safe(tmp);
}
