#ifndef _KELT_KERNEL_MEMORY_H_
#define _KELT_KERNEL_MEMORY_H_

#include "kernel/types.h"

extern char _rodata_lma;
extern char _rodata_vma;
extern char _rodata_sz;
extern char _data_lma;
extern char _data_vma;
extern char _data_sz;
extern char _bss_vma;
extern char _bss_sz;
extern char _data_brk;

#define data_brk ((void*)(&_data_brk))

void* kmemcpy(void* dst, const void* src, u32 sz);
void kmemset(void* dst, u8 fill, u32 sz);

#endif
