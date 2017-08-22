#ifndef _KELT_SCHED_SCHED_H_
#define _KELT_SCHED_SCHED_H_

#include "kernel/defs.h"
#include "kernel/types.h"
#include "kernel/list.h"
#include "drivers/nvic.h"
#include "kernel/syscall.h"

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

    struct list_node lnode;
};

/* saved task context on exception entry */
struct task_context_exc {
    u32 r0;
    u32 r1;
    u32 r2;
    u32 r3;
    u32 r12;
    u32 lr;
    u32 pc;
    u32 psr;
};

/* saved task context on context switch */
struct task_context {
    u32 r4;
    u32 r5;
    u32 r6;
    u32 r7;
    u32 r8;
    u32 r9;
    u32 r10;
    u32 r11;
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

void sched_task_set_sleeping(struct task* task);
void sched_task_wake_up(struct task* task);

#define sched_context_switch() if (sched_enabled) { NVIC_INT_CTRL_REG |= NVIC_PENDSV_SET_BIT; }

#define task_state(task) ((task)->flags & TASK_STATE_BITS)
#define task_priority(task) ((task)->flags & TASK_PRIORITY_BITS)

#define task_set_state(task, state) ((task)->flags = ((task)->flags & ~(u32)TASK_STATE_BITS) | (state))
#define task_set_priority(task, priority) ((task)->flags = ((task)->flags & ~(u32)TASK_PRIORITY_BITS) | (priority))

#define task_ctx_exc(sp) ((struct task_context_exc*)((u8*)(sp) - sizeof(struct task_context_exc)))
#define task_ctx(sp) ((struct task_context*)((u8*)(sp) - sizeof(struct task_context_exc) - sizeof(struct task_context)))

#endif
