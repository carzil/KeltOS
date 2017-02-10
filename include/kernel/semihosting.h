#ifndef _KELT_KERNEL_SEMIHOSTING_H_
#define _KELT_KERNEL_SEMIHOSTING_H_

#include "kernel/types.h"

#define SMHOST_OPEN   0x01
#define SMHOST_WRITE  0x05
#define SMHOST_WRITE0 0x04
#define SMHOST_W      0x4

extern int smhost_stdout;

void init_smh();
u32 smhost_print(void* buf, u32 size);
u32 smhost_printz(const char* buf);

#endif
