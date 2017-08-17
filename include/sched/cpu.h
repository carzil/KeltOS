#ifndef _KELT_SCHED_CPU_H_
#define _KELT_SCHED_CPU_H_

#include "kernel/defs.h"

#define MZ *1000
enum { CPU_CLOCK_RATE_HZ = 168 MZ };

#define SWITCH_MODE(from, to) asm ("mrs r0, cpsr\neor r0, %0\nmsr cpsr, r0" : : "i"(from ^ to) : "r0")
#define SET_STACK_POINTER(sp) asm ("mov sp, %0" : : "r"(sp) : "sp" )

void cpu_init();

#endif
