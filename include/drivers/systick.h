#ifndef _KELT_DRIVERS_SYSTICK_H_
#define _KELT_DRIVERS_SYSTICK_H_

#include "kernel/types.h"
#include "kernel/timer.h"

#define SYSTICK_CSR         0xe000e010
#define SYSTICK_RVR         0xe000e014
#define SYSTICK_CVR         0xe000e018
#define SYSTICK_ENABLE      0x1
#define SYSTICK_INT         0x2

extern volatile u32 c_tick;

void systick_handler();
void systick_init();

#endif
