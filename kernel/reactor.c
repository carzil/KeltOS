#include "sched/sched.h"
#include "kernel/defs.h"
#include "kernel/memory.h"
#include "kernel/bug.h"
#include "kernel/printk.h"
#include "kernel/alloc.h"
#include "kernel/reactor.h"
#include "kernel/irq.h"

struct event_type {
    s32 id;
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
    LIST_HEAD_INIT(&event_types);
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

static inline s32 can_satisfy(struct task* task)
{
    return task_state(task) == TASK_CRAVING;
}

static s32 satisfy_task(struct task* task, struct reactor_event* event, void* cb_addr)
{
    BUG_ON(task_state(task) != TASK_CRAVING);

    /* previous event has finished, cleanup stack */
    task_reset_stack(task);

    /* put event data on task stack */
    void* ev_addr = task_put_on_stack(task, event->data, event->data_size);

    /* prepare stack for context switch */
    struct sys_regs* regs = task_prepare_stack(task);
    task_fill_regs(regs, cb_addr);

    /* event callback function takes pointer to event as first argument */
    regs->r0 = (u32)ev_addr;

    /* launch task */
    sched_task_wake_up(task);

    return KELT_OK;
}

static inline struct reactor_event* event_from_options(struct reactor_event_def ev_def)
{
    struct reactor_event* ev = kalloc(sizeof(struct reactor_event));
    if (ev == NULL) {
        return NULL;
    }
    ev->type = ev_def.type_id;
    ev->data = ev_def.data;
    ev->data_size = ev_def.data_size;
    ev->destructor = ev_def.dtor;
    ev->ref_cnt = 0;
    return ev;
}

/* spread event across tasks waiting for it's type */
s32 reactor_push_event(struct reactor_event_def ev_def)
{
    struct event_type* ev_type = resolve_event_type(ev_def.type_id);

    if (ev_type == NULL) {
        return -ENOENT;
    }

    struct reactor_event* ev = event_from_options(ev_def);

    if (ev == NULL) {
        return -ENOMEM;
    }

    /* ensure no one can delete task without us */
    reactor_event_incref(ev);

    s32 err = KELT_OK;
    irq_disable_safe(__tmp);
    struct list_node* ptr;
    list_foreach(&ev_type->awaiting_tasks, ptr) {
        struct await_entry* a_entry = list_entry(ptr, struct await_entry, lnode);
        struct task* task = a_entry->task;
        BUG_ON_NULL(task);

        if (can_satisfy(task)) {
            /* no need in event queuing, satisfy task right now */
            satisfy_task(task, ev, a_entry->cb_addr);
        } else {
            /* task is not ready to serve event right now, queue it */
            struct event_queue_entry* eq_entry = kalloc(sizeof(struct event_queue_entry));
            if (eq_entry == NULL) {
                err = -ENOMEM;
                goto cleanup;
            }
            eq_entry->event = ev;
            eq_entry->cb_addr = a_entry->cb_addr;
            list_insert_last(&task->reactor_ctx.event_queue, &eq_entry->lnode);
            reactor_event_incref(ev);
        }
    }

cleanup:
    reactor_event_decref(ev);
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

struct event_queue_entry* peek_next_event(struct task* task)
{
    if (list_empty(&task->reactor_ctx.event_queue)) {
        return NULL;
    }
    struct event_queue_entry* eq_entry = list_first_entry(&task->reactor_ctx.event_queue, struct event_queue_entry, lnode);
    list_delete(&eq_entry->lnode);
    return eq_entry;
}

s32 sys_finish_event(UNUSED struct sys_regs* regs)
{
    sched_task_set_sleeping(c_task, TASK_CRAVING);
    struct event_queue_entry* eq_entry = peek_next_event(c_task);
    s32 err = KELT_OK;
    if (eq_entry != NULL) {
        /* task can be satisfied right now */
        struct reactor_event* ev = eq_entry->event;
        err = satisfy_task(c_task, ev, eq_entry->cb_addr);
        reactor_event_decref(ev);
        kfree(eq_entry);
    }
    sched_switch_task();
    return err;
}
