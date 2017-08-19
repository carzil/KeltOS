#ifndef _KELT_KERNEL_DEFS_H_
#define _KELT_KERNEL_DEFS_H_

/*
 * This headers can be also included in assembly file. Don't write any C code here!
 */

#define STACK_SIZE          0x2000
#define SRAM_BASE           0x20000000
#define SRAM_SIZE           0x20000
#define STACK_END           (SRAM_BASE + SRAM_SIZE)
#define STACK_START         (SRAM_BASE + SRAM_SIZE - STACK_START)

#define TASK_STACK_SIZE     0x1000
#define DEFAULT_PSR         0x01000000

#define RING_BUFFER_POW     12
#define RING_BUFFER_SIZE    (1 << RING_BUFFER_POW)

#define EXC_HANDLER_MAIN    0xfffffff1
#define EXC_THREAD_MAIN     0xfffffff9
#define EXC_THREAD_PROCESS  0xfffffffd

#endif
