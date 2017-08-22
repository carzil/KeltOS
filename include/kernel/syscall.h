#ifndef _KELT_KERNEL_SYSCALL_H_
#define _KELT_KERNEL_SYSCALL_H_

#include "kernel/types.h"

/* This struct defines register order on syscall entry */
struct sys_regs {
    /* registers saved by Kelt */
    u32 r4;
    u32 r5;
    u32 r6;
    u32 r7;
    u32 r8;
    u32 r9;
    u32 r10;
    u32 r11;

    /* registers saved by processor */
    u32 r0;
    u32 r1;
    u32 r2;
    u32 r3;
    u32 r12;
    u32 lr;
    u32 pc;
    u32 psr;
};

typedef s32 (*syscall_handler_t)(struct sys_regs*);

extern void swi_handler();

#endif
