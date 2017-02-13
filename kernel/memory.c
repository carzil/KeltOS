#include "kernel/memory.h"

void* kmemcpy(void* dst, const void* src, u32 sz)
{
    for (u32 i = 0; i < sz; i++) {
        *(u8*)dst++ = *(u8*)src++;
    }
    return dst;
}
