#include "kernel/syscall.h"
#include "kernel/irq.h"
#include "sched/sched.h"

void (*syscall_table[SYSCALL_COUNT])(struct sys_params* params) = {
    sys_exit
};

void syscall_handler(struct sys_params* params)
{
    syscall_table[params->sys_number](params);
}
