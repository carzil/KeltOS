#ifndef _KELT_KERNEL_ALLOC_H_
#define _KELT_KERNEL_ALLOC_H_

#include "kernel/types.h"

enum { CHUNK_FREE = 0x1, CHUNK_USED = 0x2 };

void* kalloc(u32 size);
void kfree(void* ptr);
void mm_init();

#endif
