#ifndef _KELT_KERNEL_IRQ_H_
#define _KELT_KERNEL_IRQ_H_

#define EXC_RESET 0x00000000
#define EXC_UNDOP 0x00000004
#define EXC_SWINT 0x00000008
#define EXC_ABPRF 0x0000000C
#define EXC_ABDTA 0x00000010
#define EXC_IRQ   0x00000018
#define EXC_FIQ   0x0000001C

extern void _swi_handler();
void swi_handler();

void set_exception_handler(unsigned int exc, void* addr);

#endif
