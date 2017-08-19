#ifndef _KELT_SCHED_CPU_H_
#define _KELT_SCHED_CPU_H_

#include "kernel/types.h"

#define MZ *1000
enum { CPU_CLOCK_RATE_HZ = 168 MZ };

static inline void set_psp(u32 val)
{
    asm (
        "msr psp, %0"
        :
        : "r"(val)
        :
    );
}

#endif
