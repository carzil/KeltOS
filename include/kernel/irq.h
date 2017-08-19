#ifndef _KELT_KERNEL_IRQ_H_
#define _KELT_KERNEL_IRQ_H_

#include "kernel/types.h"

/* interrupt service routine mark */
#define ISR_FUNCTION __attribute__((naked))

extern void _handler_trampoline();

#define EXCEPTION_RETURN(code) asm("bx %0" : : "r"(code) : );
#define EXCEPTION_RETURN_DEFAULT() asm("bx lr");

enum {
    ARM_THREAD_MODE   = 0xfffffff9,
    ARM_HANDLER_MODE  = 0xfffffff1,
    ARM_PROCESS_STACK = 0xfffffff5,
    ARM_MAIN_STACK    = 0xfffffff1
};

#define irq_enable() asm("cpsie if")
#define irq_disable() asm("cpsid if")

#endif
