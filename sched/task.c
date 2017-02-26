#include "sched/sched.h"
#include "kernel/alloc.h"
#include "kernel/memory.h"
#include "kernel/printk.h"
#include "kernel/bug.h"
#include "drivers/nvic.h"
#include "kernel/timer.h"
#include "kernel/syscall.h"
#include "kernel/irq.h"

int sched_enabled = 0;
struct task tasks[MAX_TASKS];
struct task* c_task = NULL;

LIST_HEAD_DEFINE(high_priotity_list);
LIST_HEAD_DEFINE(normal_priotity_list);
LIST_HEAD_DEFINE(low_priotity_list);

struct task* sched_start_task(void* start_address, int priority)
{
    struct task* task = NULL;
    for (u32 i = 0; i < MAX_TASKS; i++) {
        if (tasks[i].pid == 0) {
            task = tasks + i;
            task->pid = i + 1;
            break;
        }
    }
    if (task != NULL) {
        /* allocate only one region for all stacks? */
        void* sp_top = kalloc(TASK_STACK_SIZE);

        if (sp_top == NULL) {
            /* no space for task */
            return NULL;
        }

        void* sp = sp_top + TASK_STACK_SIZE;
        struct task_context_exc* exc_ctx = sp - sizeof(struct task_context_exc);
        struct task_context* ctx = sp - sizeof(struct task_context_exc) - sizeof(struct task_context);
        /* prepare stack pointer for first context restore */
        sp = ctx;

        /* gcc produces addresses with non-zero last bit to set EPSR.T bit */
        exc_ctx->pc = ((u32)start_address) & ~(u32)0x1;
        exc_ctx->psr = DEFAULT_PSR;

        task->sp = sp;
        task->flags = TASK_RUNNING | priority;

        switch (priority) {
        case PRIORITY_HIGH:
            list_insert_last(&high_priotity_list, &task->lnode);
            break;
        case PRIORITY_NORMAL:
            list_insert_last(&normal_priotity_list, &task->lnode);
            break;
        case PRIORITY_LOW:
            list_insert_last(&low_priotity_list, &task->lnode);
            break;
        }
    }
    return task;
}

struct task* sched_switch_task()
{
    c_task = NULL;
    if (!list_empty(&high_priotity_list)) {
        c_task = list_first_entry(high_priotity_list, struct task, lnode);
        list_rotate_left(&high_priotity_list);
    } else if (!list_empty(&normal_priotity_list)) {
        c_task = list_first_entry(normal_priotity_list, struct task, lnode);
        list_rotate_left(&normal_priotity_list);
    } else if (!list_empty(&low_priotity_list)) {
        c_task = list_first_entry(low_priotity_list, struct task, lnode);
        list_rotate_left(&low_priotity_list);
    }

    return c_task;
}

void sched_start()
{
    sched_enabled = 1;

    struct task* task = sched_switch_task();
    volatile struct task_context_exc* ctx = task->sp + sizeof(struct task_context);
    /* task has no internal state yet, so register values are not important: 
     * psr can hold actual value of pc. +1 for setting T-bit */
    ctx->psr = ctx->pc + 1;
    sched_switch_in(task);
}

void sys_exit(struct sys_params* params)
{
    BUG_ON_NULL(c_task);
    list_delete(&c_task->lnode);
    c_task->pid = 0;

    struct task* task = sched_switch_task();
    sched_return_to(task);
}
