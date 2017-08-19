#include "kernel/cdefs.h"
#include "kernel/syscall.h"
#include "kernel/irq.h"
#include "sched/sched.h"

syscall_handler_t KELT_READONLY syscall_table[] = {
    sys_exit,
    sys_yield
};
