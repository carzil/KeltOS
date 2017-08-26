#include "kernel/defs.h"
#include "kernel/syscall.h"
#include "kernel/printk.h"
#include "kernel/irq.h"
#include "sched/sched.h"
#include "kernel/timer.h"

syscall_handler_t const syscall_table[] = {
    sys_exit,
    sys_yield,
    sys_finish_event,
};

void __syscall_trampoline(struct sys_regs* regs, syscall_handler_t handler)
{
    regs->r0 = handler(regs);
    asm ("svc #0");
}

s32 __syscall_enter(struct sys_regs* regs, struct sys_exc_regs* exc_regs)
{
    u32 syscall = regs->r0;
    if (syscall >= sizeof(syscall_table) / sizeof(syscall_table[0])) {
        regs->r0 = -ENOSYS;
        return -ENOSYS;
    }

    syscall_handler_t handler = syscall_table[syscall];
    exc_regs->r0 = (u32)regs;
    exc_regs->r1 = (u32)handler;
    exc_regs->pc = (u32)__syscall_trampoline & ~(u32)0x1;
    exc_regs->psr = DEFAULT_PSR;
    c_task->saved_sp = regs;
    c_task->flags |= TASK_SYSCALL_BIT;
    return KELT_OK;
}

struct task* __syscall_exit()
{
    c_task->sp = c_task->saved_sp;
    if (c_task->flags & TASK_RESCHEDULE_BIT) {
        c_task->flags &= ~(TASK_RESCHEDULE_BIT | TASK_SYSCALL_BIT);
        return __sched_switch_task();
    }
    c_task->flags &= ~TASK_SYSCALL_BIT;
    return c_task;
}
