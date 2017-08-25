#include "kernel/timer.h"
#include "kernel/irq.h"
#include "kernel/alloc.h"
#include "sched/sched.h"
#include "drivers/semihosting.h"
#include "kernel/reactor.h"

volatile u32 c_tick = 0;

static s32 test_event_id = 0;

void timer_tick()
{
    irq_disable_safe(tmp);
    c_tick++;
    if (sched_enabled && (c_tick % 100) == 0) {
        sched_context_switch();
    }
    if (c_tick % 10000 == 0) {
        struct reactor_event_def ev_def = {
            .type_id = test_event_id,
            .dtor = NULL,
            .data = "test message",
            .data_size = sizeof("test message"),
        };
        reactor_push_event(ev_def);
    }
    irq_enable_safe(tmp);
}

void timer_init()
{
    c_tick = 0;
    test_event_id = reactor_resolve_event_type_id("org.kelt.TestEvent");
}
