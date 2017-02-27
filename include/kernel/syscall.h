#ifndef _KELT_KERNEL_SYSCALL_H_
#define _KELT_KERNEL_SYSCALL_H_

#include "kernel/types.h"

enum { SYSCALL_COUNT = 1 };

struct sys_params {
    /* params passed in registers: r0, r1, r2, r3, r12, r14 */
    u32 sys_number;
    u32 arg1;
    u32 arg2;
    u32 arg3;
    u32 arg4;
    u32 arg5;

    /* this is not syscall params but required to find parameters location */
    u32 ret_addr;
    u32 xpsr;
};

void swi_handler();
void syscall_handler(struct sys_params* params);

#endif
