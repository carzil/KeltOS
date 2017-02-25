#ifndef _KELT_KERNEL_TIMER_H_
#define _KELT_KERNEL_TIMER_H_

#include "sched/cpu.h"
#include "kernel/types.h"

#define TICK_RATE_HZ        100
#define SYSTICK_CSR         0xe000e010
#define SYSTICK_RVR         0xe000e014
#define SYSTICK_CVR         0xe000e018
#define SYSTICK_ENABLE      0x1
#define SYSTICK_INT         0x2


extern volatile u32 c_tick;

void systick_handler();
void timer_init();

#endif
