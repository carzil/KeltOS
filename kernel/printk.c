#include "kernel/printk.h"
#include "kernel/types.h"
#include "kernel/defs.h"
#include "kernel/semihosting.h"

void putc(char ch)
{
    char c[3];
    c[0] = ch;
    asm (
        "mov r0, #0x03\n"
        "mov r1, %0\n"
        "bkpt 0xab\n"
        :
        : "r"(c)
        : "r0", "r1"
    );

}
 
void puts(const char* str)
{
    smhost_printz(str);
}

void printu32(u32 a)
{
    char buf[12];
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

