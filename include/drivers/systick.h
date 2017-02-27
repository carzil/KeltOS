#ifndef _KELT_DRIVERS_SYSTICK_H_
#define _KELT_DRIVERS_SYSTICK_H_

#include "kernel/types.h"
#include "kernel/timer.h"

enum {
    SYSTICK_CSR    = 0xe000e010,
    SYSTICK_RVR    = 0xe000e014,
    SYSTICK_CVR    = 0xe000e018,
    SYSTICK_ENABLE = 0x1,
    SYSTICK_INT    = 0x2
};

extern volatile u32 c_tick;

void systick_handler();
void systick_init();

#endif
