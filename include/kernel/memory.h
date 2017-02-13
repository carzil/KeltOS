#ifndef _KELT_KERNEL_MEMORY_H_
#define _KELT_KERNEL_MEMORY_H_

#include "kernel/types.h"

extern void* data_brk;

extern void* _rodata_lma;
extern void* _rodata_vma;
extern void* _rodata_sz;
extern void* _data_lma;
extern void* _data_vma;
extern void* _data_sz;
extern void* _bss_vma;
extern void* _bss_sz;
extern void* _data_brk;

void* kmemcpy(void* dst, const void* src, u32 sz);

#endif
