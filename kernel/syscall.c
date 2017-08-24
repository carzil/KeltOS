#include "kernel/defs.h"
#include "kernel/syscall.h"
#include "kernel/irq.h"
#include "sched/sched.h"

syscall_handler_t KELT_READONLY syscall_table[] = {
    sys_exit,
    sys_yield,
    sys_finish_event
};

void __syscall_trampoline(struct sys_regs* regs)
{
    u32 syscall = regs->r0;
    if (syscall >= sizeof(syscall_table) / sizeof(syscall_table[0])) {
        regs->r0 = -ENOSYS;
        return;
    }

    syscall_handler_t handler = syscall_table[syscall];
    regs->r0 = handler(regs);
}
