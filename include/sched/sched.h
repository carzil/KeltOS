#ifndef _KELT_SCHED_SCHED_H_
#define _KELT_SCHED_SCHED_H_

#include "kernel/defs.h"
#include "kernel/types.h"
#include "kernel/list.h"
#include "drivers/nvic.h"
#include "kernel/syscall.h"

#define MAX_TASKS           16
#define TASK_RUNNING        0x1
#define PRIORITY_HIGH       0x12
#define PRIORITY_NORMAL     0x8
#define PRIORITY_LOW        0x4

struct task {
    void* sp;

    u32 pid;
    /*
     * bits 0-2: task state (running, blocked etc)
     * bits 3-4: task priority
     */
    u32 flags;
    
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
struct task* sched_switch_task();
void sys_exit(struct sys_params* params);

void sys_sched_start(struct sys_params* params);
void sched_return_to(struct task* task);
void sched_switch_in(struct task* task);
#define sched_context_switch() if (sched_enabled) { NVIC_INT_CTRL_REG |= NVIC_PENDSV_SET_BIT; }

#endif
