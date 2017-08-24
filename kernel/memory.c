#include "kernel/memory.h"

void* kmemcpy(void* dst, const void* src, u32 sz)
{
    for (u32 i = 0; i < sz; i++) {
        *(u8*)dst++ = *(u8*)src++;
    }
    return dst;
}

void kmemset(void* dst, u8 fill, u32 sz)
{
    for (u32 i = 0; i < sz; i++) {
        *(u8*)dst++ = fill;
    }
}

size_t strlen(const char* str)
{
    size_t cnt = 0;
    for (; str[cnt] != '\0'; cnt++);
    return cnt;
}

size_t strnlen(const char* str, size_t maxlen)
{
    size_t cnt = 0;
    for (; (cnt < maxlen) && str[cnt]; cnt++);
    return cnt;
}

int strcmp(const char* a, const char* b)
{
    while (*a && (*a == *b)) {
        a++;
        b++;
    }
    return *(const u8*)a - *(const u8*)b;
}
