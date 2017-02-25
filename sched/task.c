#include "sched/sched.h"
#include "kernel/alloc.h"
#include "kernel/memory.h"
#include "kernel/printk.h"
#include "kernel/bug.h"
#include "drivers/nvic.h"
#include "kernel/timer.h"

int sched_enabled = 0;
struct task tasks[MAX_TASKS];
struct task* c_task;

struct task* sched_start_task(void* start_address)
{
    struct task* task = NULL;
    for (u32 i = 0; i < MAX_TASKS; i++) {
        if (tasks[i].pid == 0) {
            task = tasks + i;
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

        printk("sp_top: %p, ctx: %p, exc_ctx: %p\n", sp_top, ctx, exc_ctx);
        task->sp = sp;
        task->flags = TASK_RUNNING;
        task->pid = 1;
    }
    return task;
}

struct task* sched_switch_task()
{
    for (u32 i = 0; i < MAX_TASKS; i++) {
        if (tasks[i].flags & TASK_RUNNING) {
            if (tasks + i != c_task) {
                c_task = tasks + i;
                return c_task;
            }
        }
    }
    return NULL;
}

void sched_start()
{
    asm (
        "mov    r0, #0\n"
        "swi    #32\n"

    );
}
