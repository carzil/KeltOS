#pragma once

#include "kernel/types.h"

#define _STRINGIFY(x) #x
#define STRINGIFY(x) _STRINGIFY(x)

#define UNUSED      __attribute__((unused))

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
