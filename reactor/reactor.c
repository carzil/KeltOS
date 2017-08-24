#include "sched/sched.h"
#include "kernel/defs.h"
#include "kernel/memory.h"
#include "kernel/bug.h"
#include "kernel/alloc.h"
#include "reactor/reactor.h"
#include "kernel/irq.h"

struct event_type {
    u32 id;
    char name[MAX_EVENT_TYPE_NAME_SIZE + 1];

    LIST_HEAD_DECLARE(awaiting_tasks);
    struct list_node lnode;
};

struct await_entry {
    struct task* task;
    void* cb_addr;
    struct list_node lnode;
};

struct event_queue_entry {
    struct reactor_event* event;
    void* cb_addr;
    struct list_node lnode;
};

static s32 free_ev_type_id = 0;
static LIST_HEAD_DEFINE(event_types);

static struct event_type* resolve_event_type(s32 id)
{
    struct list_node* ptr;
    list_foreach(&event_types, ptr) {
        struct event_type* ev_type = list_entry(ptr, struct event_type, lnode);
        if (ev_type->id == id) {
            return ev_type;
        }
    }
    return NULL;
}

void reactor_init()
{
}

void reactor_task_init(struct task* task)
{
    LIST_HEAD_INIT(&task->reactor_ctx.event_queue);
}

/* registers new event type in reactor */
s32 reactor_register_event_type(const char* name)
{
    size_t len = strnlen(name, MAX_EVENT_TYPE_NAME_SIZE + 1);
    if (len == MAX_EVENT_TYPE_NAME_SIZE + 1) {
        return -EINVAL;
    }
    if (reactor_resolve_event_type_id(name) >= 0) {
        return -EEXIST;
    }
    struct event_type* ev_type = kalloc(sizeof(struct event_type));
    kmemcpy(&ev_type->name, name, len);
    ev_type->name[len] = '\0';
    ev_type->id = free_ev_type_id++;
    LIST_HEAD_INIT(&ev_type->awaiting_tasks);
    list_insert_last(&event_types, &ev_type->lnode);
    return ev_type->id;
}

/* returns event type id by its name */
s32 reactor_resolve_event_type_id(const char* name)
{
    struct list_node* ptr;
    list_foreach(&event_types, ptr) {
        struct event_type* ev_type = list_entry(ptr, struct event_type, lnode);
        if (strcmp(ev_type->name, name) == 0) {
            return ev_type->id;
        }
    }
    return -ENOENT;
}

static s32 _try_appease_task(struct task* task)
{
    if (task_state(task) != TASK_CRAVING) {
        return KELT_OK;
    }
    struct event_queue_entry* eq_entry = list_first_entry(&task->reactor_ctx.event_queue, struct event_queue_entry, lnode);
    struct reactor_event* event = eq_entry->event;

    /* previous event has finished, cleanup stack */
    task_reset_stack(task);

    /* put event on task stack */
    void* ev_addr = task_put_on_stack(task, event, event->size);

    /* prepare stack for context switch */
    struct sys_regs* regs = task_prepare_stack(task);
    task_fill_regs(regs, eq_entry->cb_addr);

    /* event callback function takes pointer to event as first argument */
    regs->r0 = (u32)ev_addr;

    /* launch task */
    sched_task_wake_up(task);

    /* delete event from event queue */
    list_delete(&eq_entry->lnode);
    kfree(eq_entry);
    event->ref_cnt--;

    return KELT_OK;
}

/* spread event across tasks waiting for it's type */
s32 reactor_push_event(struct reactor_event* ev)
{
    if (ev == NULL) {
        return -EINVAL;
    }

    s32 err = KELT_OK;
    struct event_type* ev_type = resolve_event_type(ev->type);
    if (ev_type == NULL) {
        err = -ENOENT;
        goto cleanup;
    }

    irq_disable_safe(__tmp);
    struct list_node* ptr;
    list_foreach(&ev_type->awaiting_tasks, ptr) {
        struct await_entry* a_entry = list_entry(ptr, struct await_entry, lnode);
        struct task* task = a_entry->task;
        BUG_ON_NULL(task);
        struct event_queue_entry* eq_entry = kalloc(sizeof(struct event_queue_entry));
        if (eq_entry == NULL) {
            err = -ENOMEM;
            goto cleanup;
        }
        eq_entry->event = ev;
        eq_entry->cb_addr = a_entry->cb_addr;
        list_insert_last(&task->reactor_ctx.event_queue, &eq_entry->lnode);
        reactor_event_incref(ev);
        _try_appease_task(task);
    }

cleanup:
    reactor_event_cleanup(ev);

    irq_enable_safe(__tmp);
    return err;
}

/* register specified task for given event_type */
s32 reactor_watch_for(struct task* task, u32 ev_type_id, void* addr)
{
    struct event_type* ev_type = resolve_event_type(ev_type_id);
    if (ev_type == NULL) {
        return -ENOENT;
    }
    struct await_entry* entry = kalloc(sizeof(struct await_entry));
    entry->task = task;
    entry->cb_addr = addr;
    list_insert_last(&ev_type->awaiting_tasks, &entry->lnode);
    return KELT_OK;
}

s32 sys_finish_event(struct sys_regs* regs)
{
    sched_task_set_sleeping(c_task, TASK_CRAVING);
    sched_switch_task();
    return KELT_OK;
}
