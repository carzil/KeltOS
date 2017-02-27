#ifndef _KELT_KERNEL_SEMIHOSTING_H_
#define _KELT_KERNEL_SEMIHOSTING_H_

#include "kernel/types.h"
enum {
    SMHOST_OPEN   = 0x01,
    SMHOST_WRITE  = 0x05,
    SMHOST_WRITE0 = 0x04,
    SMHOST_W      = 0x4
};

extern int smhost_stdout;

u32 smhost_print(void* buf, u32 size);
u32 smhost_printz(const char* buf);

#endif
