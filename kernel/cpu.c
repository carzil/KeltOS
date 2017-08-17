#include "sched/cpu.h"
#include "kernel/types.h"


/*
 * It is a temporary stack for irq/fiq mode. In ISR we'll save those registers
 * and then switch to SVC mode. That is done for ISRs preemption.
 */
struct temp_stack {
    u32 r0;
    u32 lr;
    u32 cpsr;
};
struct temp_stack _temp_stacks[2];

/*
 * This routine *MUST* be called from SVC mode
 */
void cpu_init()
{
    SWITCH_MODE(SVC_MODE, IRQ_MODE);
    SET_STACK_POINTER(&_temp_stacks[0]);
    SWITCH_MODE(IRQ_MODE, FIQ_MODE);
    SET_STACK_POINTER(&_temp_stacks[1]);
    SWITCH_MODE(FIQ_MODE, SVC_MODE);
}
