#ifndef _KELT_DRIVERS_TIMER_H_
#define _KELT_DRIVERS_TIMER_H_

#include "kernel/types.h"

#define TIMER_32_BIT                (1u << 1u)
#define TIMER_IRQ_ENABLE            (1u << 5u)
#define TIMER_PERIODIC              (1u << 6u)
#define TIMER_ENABLE                (1u << 7u)
#define TIMER_ONESHOT               1u
#define TIMER_PRESCALE_4            (1u << 2u)
#define TIMER_PRESCALE_8            (2u << 2u)
#define TIMER_IRQ                   4u

struct _hw_timer {
    u32 load_reg;
    u32 value_reg;
    u32 ctl_reg;
    u32 intclr_reg;
    u32 ris_reg;
    u32 mis_reg;
    u32 bgload_reg;
};

static struct _hw_timer* timer0 = (struct _hw_timer*)(0x101e2000);

static inline void timer_ctl(struct _hw_timer* timer, u32 flags)
{
    timer->ctl_reg = flags;
}

static inline void timer_load(struct _hw_timer* timer, u32 load)
{
    timer->load_reg = load;
}

static inline void timer_clear_interrupt(struct _hw_timer* timer)
{
    /* any writes to this register will clear interrupt */
    timer->intclr_reg = 0;
}

#endif
