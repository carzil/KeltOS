#include "printk.h"

#define SERIAL_BASE 0x16000000
#define SERIAL_FLAG_REGISTER 0x18
#define SERIAL_BUFFER_FULL (1 << 5)
 
void putc(char c)
{
    /* Wait until the serial buffer is empty */
    while (*(volatile unsigned long*)(SERIAL_BASE + SERIAL_FLAG_REGISTER) 
                                       & (SERIAL_BUFFER_FULL));
    /* Put our character, c, into the serial buffer */
    *(volatile unsigned long*)SERIAL_BASE = c;
 
    /* Print a carriage return if this is a newline, as the cursor's x position will not reset to 0*/
    if (c == '\n')
    {
        putc('\r');
    }
}
 
void puts(const char* str)
{
    while (*str) {
        putc(*str++);
    }
}

void printu32(int a)
{
    char buf[12];
    int i;
    for (i = 10; a > 0; i--) {
        buf[i] = (a % 10) + '0';
        a /= 10;
    }
    buf[11] = '\0';
    puts(buf + i + 1);
}
