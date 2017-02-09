#include "kernel/printk.h"
#include "kernel/types.h"

#define UART0_BASE 0x4000c000
 
void putc(u32 ch)
{
    while ((*(volatile u32*)UART0_BASE) & 7);
    *(volatile u32*)UART0_BASE = ch;
}
 
void puts(const u8* str)
{
    while (*str) {
        putc(*str++);
    }
}

void printu32(u32 a)
{
    u8 buf[12];
    u32 i;

    if (a == 0) {
        putc('0');
        return;
    }
    for (i = 10; a > 0; i--) {
        buf[i] = (a % 10) + '0';
        a /= 10;
    }
    buf[11] = '\0';
    puts(buf + i + 1);
}

