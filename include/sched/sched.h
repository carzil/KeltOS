#ifndef _KELT_SCHED_SCHED_H_
#define _KELT_SCHED_SCHED_H_

#include "kernel/defs.h"
#include "kernel/types.h"
#include "kernel/list.h"
#include "drivers/nvic.h"
#include "kernel/syscall.h"
#include "kernel/reactor.h"

enum {
    MAX_TASKS           = 16,
    INVALID_PID         = 0,
    MAX_PRIORITY        = 2,

    TASK_STATE_BITS     = 0b11,
    TASK_SLEEPING       = 0b00,
    TASK_RUNNING        = 0b01,
    TASK_CRAVING        = 0b10,

    TASK_PRIORITY_BITS  = 0b1100,
    PRIORITY_HIGH       = 0b0000,
    PRIORITY_NORMAL     = 0b0100,
    PRIORITY_LOW        = 0b1000,
};

struct task {
    void* sp;
    u32 flags;

    void* sp_initial;
    char* name;
    u32 pid;

    struct reactor_task_ctx reactor_ctx;

    struct list_node lnode;
};

/* stores current task */
extern struct task* c_task;
extern int sched_enabled;
extern struct task tasks[MAX_TASKS];

extern void context_switch(struct task* from, struct task* to);

struct task* sched_start_task(void* start_address, int priority);
void sched_start();
void sched_init();
struct task* sched_switch_task();

s32 sys_exit(struct sys_regs* regs);
s32 sys_yield(struct sys_regs* regs);

void sched_return_to(struct task* task);
void sched_switch_in(struct task* task);

void sched_task_set_sleeping(struct task* task, u32 state);
void sched_task_wake_up(struct task* task);

void* task_put_on_stack(struct task* task, void* data, size_t size);
struct sys_regs* task_prepare_stack(struct task* task);

struct task* sched_create_task(int priority);

#define sched_context_switch() if (sched_enabled) { NVIC_INT_CTRL_REG |= NVIC_PENDSV_SET_BIT; }

#define task_state(task) ((task)->flags & TASK_STATE_BITS)
#define task_priority(task) ((task)->flags & TASK_PRIORITY_BITS)

#define task_set_state(task, state) ((task)->flags = ((task)->flags & ~(u32)TASK_STATE_BITS) | (state))
#define task_set_priority(task, priority) ((task)->flags = ((task)->flags & ~(u32)TASK_PRIORITY_BITS) | (priority))

static inline void task_fill_regs(struct sys_regs* regs, void* addr)
{
    /* gcc produces addresses with non-zero last bit to set EPSR.T bit */
    regs->pc = (u32)addr & ~(u32)0x1;
    regs->psr = DEFAULT_PSR;
}

static inline void task_reset_stack(struct task* task)
{
    task->sp = task->sp_initial;
}

#endif
