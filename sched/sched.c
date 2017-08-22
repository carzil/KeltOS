#include "kernel/defs.h"
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

LIST_HEAD_DECLARE(priority_lists)[MAX_PRIORITY + 1];

/* TODO: current realization lacks synchoronization */

static void sched_insert_task(struct task* task)
{
    size_t pos = task_priority(task) >> 2;
    list_insert_last(&priority_lists[pos], &task->lnode);
}

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
            /* no space for new task */
            return NULL;
        }

        task->sp_initial = sp_top;
        task->sp = sp_top + TASK_STACK_SIZE;
        struct task_context_exc* exc_ctx = task_ctx_exc(task->sp);
        struct task_context* ctx = task_ctx(task->sp);
        /* prepare stack pointer for first context restore */
        task->sp = ctx;

        /* gcc produces addresses with non-zero last bit to set EPSR.T bit */
        exc_ctx->pc = ((u32)start_address) & ~(u32)0x1;
        exc_ctx->psr = DEFAULT_PSR;

        task_set_priority(task, priority);
        task_set_state(task, TASK_RUNNING);

        sched_insert_task(task);
    }

    return task;
}

void sched_finish_task(struct task* task)
{
    if (task_state(task) == TASK_RUNNING) {
        list_delete(&task->lnode);
    }
    kfree(task->sp_initial);
    task->pid = INVALID_PID;
}

struct task* sched_switch_task()
{
    c_task = NULL;
    for (size_t i = 0; i <= MAX_PRIORITY; i++) {
        if (!list_empty(&priority_lists[i])) {
            c_task = list_first_entry(&priority_lists[i], struct task, lnode);
            list_rotate_left(&priority_lists[i]);
            break;
        }
    }

    return c_task;
}

void sched_task_set_sleeping(struct task* task)
{
    list_delete(&task->lnode);
    task_set_state(task, TASK_SLEEPING);
}

void sched_task_wake_up(struct task* task)
{
    if (task_state(task) == TASK_SLEEPING) {
        task_set_state(task, TASK_RUNNING);
        sched_insert_task(task);
    }
}

s32 sys_exit(struct sys_regs* params)
{
    BUG_ON_NULL(c_task);
    sched_finish_task(c_task);
    sched_context_switch();
    return KELT_OK;
}

s32 sys_yield(struct sys_regs* params)
{
    BUG_ON_NULL(c_task);
    sched_switch_task();
    return KELT_OK;
}

void sched_init()
{
    for (size_t i = 0; i <= MAX_PRIORITY; i++) {
        LIST_HEAD_INIT(&priority_lists[i]);
    }
}

void sched_start()
{
    c_task = sched_switch_task();

    /*
     * PendSV handler assumes that there is
     * real task already running, so we need to emulate
     * CPU behavior on exception entry.
     */
    c_task->sp += sizeof(struct task_context);
    set_psp((u32)c_task->sp);

    sched_enabled = 1;
}
