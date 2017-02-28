#include "kernel/syscall.h"
#include "kernel/irq.h"
#include "sched/sched.h"

syscall_handler_t syscall_table[SYSCALL_COUNT] = {
    sys_exit,
    sys_yield
};
