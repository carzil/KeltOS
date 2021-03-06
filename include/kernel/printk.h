#ifndef _KELT_KERNEL_PRINTK_H_
#define _KELT_KERNEL_PRINTK_H_

#include "types.h"

void printk(const char* fmt, ...);
void dump_kernel_log_task();
void printk_init();

#ifdef DEBUG
#define printk_debug(args) printk(args)
#else
#define print_debug()
#endif

#endif
