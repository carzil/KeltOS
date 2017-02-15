#ifndef _KELT_KERNEL_PRINTK_H_
#define _KELT_KERNEL_PRINTK_H_

#include "types.h"

void printu32(u32 a);
void putc(char c);
void puts(const char* str);
void printk(const char* fmt, ...);

#endif
