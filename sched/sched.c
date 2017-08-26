#include "kernel/defs.h"
#include "sched/sched.h"
#include "kernel/alloc.h"
#include "kernel/memory.h"
#include "kernel/printk.h"
#include "kernel/panic.h"
#include "drivers/nvic.h"
#include "kernel/timer.h"
#include "kernel/syscall.h"
#include "kernel/irq.h"

int sched_enabled = 0;
struct task tasks[MAX_TASKS];
struct task idle_task = {
    .name = "idle task",
    .flags = TASK_RUNNING | PRIORITY_HIGH,
    .pid = INVALID_PID,
};
struct task* c_task = NULL;

LIST_HEAD_DECLARE(priority_lists)[MAX_PRIORITY + 1];

/* TODO: current realization lacks synchoronization */

static void sched_insert_task(struct task* task)
{
    size_t pos = task_priority(task) >> 2;
    list_insert_last(&priority_lists[pos], &task->lnode);
}

struct sys_regs* task_prepare_stack(struct task* task)
{
    task->sp -= sizeof(struct sys_regs);
    return task->sp;
}

void* task_put_on_stack(struct task* task, void* data, size_t sz)
{
    task->sp -= sz;
    kmemcpy(task->sp, data, sz);
    return task->sp;
}

struct task* sched_create_task(int priority)
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
        void* sp_top = kalloc(TASK_STACK_SIZE + TASK_KSTACK_SIZE);

        if (sp_top == NULL) {
            return NULL;
        }

        task->sp_initial = sp_top + TASK_STACK_SIZE;
        task->ksp = sp_top + TASK_STACK_SIZE + TASK_KSTACK_SIZE;
        task->sp = task->sp_initial;
        task_prepare_stack(task);
        task_set_priority(task, priority);
        task_set_state(task, TASK_CRAVING);

        task->flags &= ~(u32)TASK_SYSCALL_BIT;

        reactor_task_init(task);
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

struct task* __sched_switch_task()
{
    c_task = NULL;
    irq_disable_safe(tmp);
    for (size_t i = 0; i <= MAX_PRIORITY; i++) {
        if (!list_empty(&priority_lists[i])) {
            c_task = list_first_entry(&priority_lists[i], struct task, lnode);
            list_rotate_left(&priority_lists[i]);
            break;
        }
    }
    if (c_task == NULL) {
        c_task = &idle_task;
    }
    irq_enable_safe(tmp);
    return c_task;
}

void sched_task_set_sleeping(struct task* task, u32 state)
{
    list_delete(&task->lnode);
    task_set_state(task, state);
}

void sched_task_wake_up(struct task* task)
{
    task_set_state(task, TASK_RUNNING);
    sched_insert_task(task);
}

s32 sys_exit(UNUSED struct sys_regs* params)
{
    BUG_ON_NULL(c_task);
    sched_finish_task(c_task);
    sched_context_switch();
    return KELT_OK;
}

s32 sys_yield(UNUSED struct sys_regs* params)
{
    BUG_ON_NULL(c_task);
    sched_context_switch();
    return KELT_OK;
}

void NAKED _do_idle_task()
{
    for (;;) {
        asm ("wfi");
    }
    BUG_ON_REACH();
}

void sched_reschedule()
{
    c_task->flags |= TASK_RESCHEDULE_BIT;
}

void sched_init()
{
    for (size_t i = 0; i <= MAX_PRIORITY; i++) {
        LIST_HEAD_INIT(&priority_lists[i]);
    }
    idle_task.sp = kalloc(sizeof(struct sys_regs)) + sizeof(struct sys_regs);
    task_prepare_stack(&idle_task);
    task_fill_regs(idle_task.sp, &_do_idle_task);

    /* TODO: move to nvic initialization */
    *(volatile u32*)0xe000ed20 |= 0x1 << 24;
    *(volatile u32*)0xe000ed20 |= 0x2 << 16;
}

void sched_start()
{
    sched_enabled = 1;
    idle_task.ksp = kalloc(TASK_KSTACK_SIZE);
    __sched_switch_task();
    c_task->saved_sp = c_task->sp;
    irq_enable_force();
    set_msp((u32)c_task->ksp);
    asm ("svc 0");
    BUG_ON_REACH();
}
