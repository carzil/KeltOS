#include "kernel/timer.h"
#include "kernel/irq.h"
#include "kernel/alloc.h"
#include "sched/sched.h"
#include "drivers/semihosting.h"
#include "reactor/reactor.h"

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
        struct reactor_event* ev = kalloc(sizeof(struct reactor_event));
        *ev = REACTOR_EVENT_INITIALIZER(test_event_id, struct reactor_event, NULL);
        reactor_push_event(ev);
    }
    irq_enable_safe(tmp);
}

void timer_init()
{
    c_tick = 0;
    test_event_id = reactor_resolve_event_type_id("org.kelt.TestEvent");
}
