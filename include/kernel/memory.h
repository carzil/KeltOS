#ifndef _KELT_KERNEL_MEMORY_H_
#define _KELT_KERNEL_MEMORY_H_

#include "kernel/types.h"

extern void* _data_load;
extern void* _data_start;
extern void* _data_end;

extern void* _bss_start;
extern void* _bss_end;
extern void* _data_brk;

#define data_brk ((void*)(&_data_brk))

void* kmemcpy(void* dst, const void* src, u32 sz);
void kmemset(void* dst, u8 fill, u32 sz);
size_t strlen(const char* str);

#endif
