#include "kernel/syscall.h"
#include "kernel/irq.h"
#include "sched/sched.h"

void sys_sched_start(struct sys_params* params);

void (*syscall_table[SYSCALL_COUNT])(struct sys_params* params) = {
    sys_sched_start,
};

void syscall_handler(struct sys_params* params)
{
    syscall_table[params->sys_number](params);
}
