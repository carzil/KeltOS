#ifndef _KELT_SCHED_CPU_H_
#define _KELT_SCHED_CPU_H_

#define MZ *1000
#define CPU_CLOCK_RATE_HZ 168 MZ

static inline int __get_cpsr()
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

#endif
