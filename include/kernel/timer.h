#ifndef _KELT_KERNEL_TIMER_H_
#define _KELT_KERNEL_TIMER_H_

#include "sched/cpu.h"
#include "kernel/types.h"

#define TICK_RATE_HZ        100

extern volatile u32 c_tick;

#endif
