#ifndef _KELT_KERNEL_SYSCALL_H_
#define _KELT_KERNEL_SYSCALL_H_

#include "kernel/defs.h"

typedef s32 (*syscall_handler_t)(struct sys_regs*);

extern void swi_handler();

#endif
