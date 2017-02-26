#ifndef _KELT_KERNEL_IRQ_H_
#define _KELT_KERNEL_IRQ_H_

#include "kernel/types.h"

/* interrupt service routine mark */
#define ISR_FUNCTION __attribute__((naked))

extern void _handler_trampoline();

#define EXCEPTION_RETURN(code) asm("mov lr, %0\nbx lr\n" : : "r"(code) : );
#define EXCEPTION_RETURN_DEFAULT() asm("bx lr");

#define ARM_THREAD_MODE     0xfffffff9
#define ARM_HANDLER_MODE    0xfffffff1
#define ARM_PROCESS_STACK   0xfffffff5
#define ARM_MAIN_STACK      0xfffffff1

#endif
