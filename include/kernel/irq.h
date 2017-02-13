#ifndef _KELT_KERNEL_IRQ_H_
#define _KELT_KERNEL_IRQ_H_

#include "kernel/types.h"

extern void _handler_trampoline();
u32 swi_handler();

#endif
