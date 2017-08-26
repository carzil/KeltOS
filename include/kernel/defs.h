#ifndef _KELT_KERNEL_DEFS_H_
#define _KELT_KERNEL_DEFS_H_

#define STACK_SIZE          0x400
#define SRAM_START          0x20000000
#define SRAM_SIZE           (128 * 1024)
#define SRAM_END            (SRAM_START + SRAM_SIZE)

#define TASK_STACK_SIZE     0x700
#define TASK_KSTACK_SIZE    0x300
#define DEFAULT_PSR         (1 << 24)

#define RING_BUFFER_POW     12
#define RING_BUFFER_SIZE    (1 << RING_BUFFER_POW)

#define EXC_HANDLER_MAIN    0xfffffff1
#define EXC_THREAD_MAIN     0xfffffff9
#define EXC_THREAD_PROCESS  0xfffffffd

#define TASK_RESCHEDULE_BIT 0b010000
#define TASK_SYSCALL_BIT    0b100000

#define KELT_OK             0
#define ENOSYS              1
#define EINVAL              2
#define ENOENT              3
#define EEXIST              4
#define ENOMEM              5


#ifdef __ASSEMBLER__

#include "kernel/defs_asm.h"

#else

#include "kernel/defs_c.h"

#endif

#endif
