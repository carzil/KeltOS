#ifndef _KELT_CPU_H_
#define _KELT_CPU_H_

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
