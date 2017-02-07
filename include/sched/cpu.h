#ifndef _KELT_CPU_H_
#define _KELT_CPU_H_

extern void __switch_to_system_mode();

static inline int __get_cpsr()
{
    int cpsr;
    asm (
        "mrs %0, cpsr"
        : "=r"(cpsr)
        :
        :
    );
    return cpsr;
}

#endif
