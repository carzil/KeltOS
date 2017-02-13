#ifndef _KELT_KERNEL_DEFS_H_
#define _KELT_KERNEL_DEFS_H_

/*
 * This headers can be also included in assembly file. Don't write any C code here!
 */

#define STACK_SIZE      0x2000
#define SRAM_BASE       0x20000000
#define SRAM_SIZE       0x20000
#define STACK_END       (SRAM_BASE + SRAM_SIZE)
#define STACK_START     (SRAM_BASE + SRAM_SIZE - STACK_START)

#endif
