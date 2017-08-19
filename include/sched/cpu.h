#ifndef _KELT_SCHED_CPU_H_
#define _KELT_SCHED_CPU_H_

#include "kernel/types.h"

#define MZ *1000
enum { CPU_CLOCK_RATE_HZ = 168 MZ };

static inline int get_cpsr()
{
    int cpsr;
    asm (
        "mrs %0, psr"
        : "=r"(cpsr)
        :
        :
    );
    return cpsr;
}

static inline void set_cpsr(u32 val)
{
    asm (
        "msr psr, %0"
        :
        : "r"(val)
        :
    );
}

static inline void set_control_reg(u32 val)
{
    asm (
        "msr control, %0"
        :
        : "r"(val)
        :
    );
}

#endif
