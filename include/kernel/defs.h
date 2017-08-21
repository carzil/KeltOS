#ifndef _KELT_KERNEL_DEFS_H_
#define _KELT_KERNEL_DEFS_H_

#define STACK_SIZE          0x2000
#define SRAM_BASE           0x20000000
#define SRAM_SIZE           (128 * 1024)
#define STACK_END           (SRAM_BASE + SRAM_SIZE)
#define STACK_START         (SRAM_BASE + SRAM_SIZE - STACK_START)

#define TASK_STACK_SIZE     0x1000
#define DEFAULT_PSR         (1 << 24)

#define RING_BUFFER_POW     12
#define RING_BUFFER_SIZE    (1 << RING_BUFFER_POW)

#define EXC_HANDLER_MAIN    0xfffffff1
#define EXC_THREAD_MAIN     0xfffffff9
#define EXC_THREAD_PROCESS  0xfffffffd

#define KELT_OK             0


#ifdef __ASSEMBLER__

#include "kernel/defs_asm.h"

#else

#include "kernel/defs_c.h"

#endif

#endif
