#ifndef _KELT_KERNEL_PRINTK_H_
#define _KELT_KERNEL_PRINTK_H_

#include "types.h"

void printu32(u32 a);
void putc(u32 c);
void puts(const u8* str);

#endif
