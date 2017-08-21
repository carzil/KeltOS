#ifndef _KELT_KERNEL_IRQ_H_
#define _KELT_KERNEL_IRQ_H_

#include "kernel/types.h"

/* interrupt service routine mark */
#define ISR_FUNCTION __attribute__((naked))

extern void _handler_trampoline();

#define EXCEPTION_RETURN(code) asm("bx %0" : : "r"(code) : );
#define EXCEPTION_RETURN_DEFAULT() asm("bx lr");

#define irq_enable_force() asm("cpsie i")
#define irq_disable_force() asm("cpsid i")

#define irq_disable_safe(var)  \
    u32 var = 0;               \
    asm(                       \
        "mrs    %0, PRIMASK\n" \
        "cpsid  i\n"           \
        : "=r"(var)            \
        :                      \
        :                      \
    )

#define irq_enable_safe(var)   \
    asm(                       \
        "msr    PRIMASK, %0\n" \
        :                      \
        : "r"(var)             \
        :                      \
    )

#define isb() asm("isb")

#endif
