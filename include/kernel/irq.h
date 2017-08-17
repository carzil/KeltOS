#ifndef _KELT_KERNEL_IRQ_H_
#define _KELT_KERNEL_IRQ_H_

#include "kernel/types.h"

/* interrupt service routine mark */
#define ISR_FUNCTION __attribute__((naked))

enum {
    ARM_THREAD_MODE   = 0xfffffff9,
    ARM_HANDLER_MODE  = 0xfffffff1,
    ARM_PROCESS_STACK = 0xfffffff5,
    ARM_MAIN_STACK    = 0xfffffff1
};

#define save_lr() asm("push {lr}")
#define restore_lr() asm("pop {lr}")

#define enable_irq() asm("cpsie i")
#define disable_irq() asm("cpsid i")

static inline int irq_enabled()
{
    int t;
    asm (
        "mrs %[t], cpsr"
        : [t]"=r"(t)
        :
        :
    );

    return !(t & (1 << 7));
}

#endif
