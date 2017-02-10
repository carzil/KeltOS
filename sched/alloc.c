#include "kernel/alloc.h"
#include "kernel/types.h"
#include "kernel/printk.h"

extern u32 _edata;

void* kalloc(u32 size)
{
    printu32(_edata);
    putc('\n');
    return (void*)0;
}
