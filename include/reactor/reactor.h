#pragma once

#include "kernel/types.h"
#include "kernel/list.h"

#define MAX_EVENT_TYPE_NAME_SIZE    50
#define INVALID_EVENT_TYPE_ID       -1

struct task;
struct reactor_event;
typedef void (*event_destructor_t)(struct reactor_event*);

struct reactor_event_def {
    s32 type_id;
    void* data;
    size_t data_size;
    event_destructor_t dtor;
};

struct reactor_event {
    s32 type;
    s32 ref_cnt;

    size_t data_size;
    void* data;

    event_destructor_t destructor;

    struct list_node lnode;
};

#define REACTOR_EVENT_INITIALIZER(type_id, type, destructor) ((struct reactor_event) { type_id, 0, sizeof(type), destructor, { NULL, NULL } })

struct reactor_task_ctx {
    LIST_HEAD_DECLARE(event_queue);
};

void reactor_init();
void reactor_task_init(struct task* task);
s32 reactor_register_event_type(const char* name);
s32 reactor_resolve_event_type_id(const char* name);
s32 reactor_push_event(struct reactor_event_def ev_def);
s32 reactor_watch_for(struct task* task, u32 ev_type_id, void* addr);
s32 sys_finish_event(struct sys_regs* regs);

static inline void reactor_event_init(struct reactor_event* ev)
{
    ev->ref_cnt = 0;
    ev->type = INVALID_EVENT_TYPE_ID;
    ev->destructor = (event_destructor_t)NULL;
}

static inline void reactor_event_incref(struct reactor_event* ev)
{
    ev->ref_cnt++;
}

static inline void reactor_event_decref(struct reactor_event* ev)
{
    if (--ev->ref_cnt == 0) {
        if (ev->destructor) {
            ev->destructor(ev);
        }
    }
}

static inline void reactor_event_cleanup(struct reactor_event* ev)
{
    if (ev->ref_cnt == 0) {
        if (ev->destructor) {
            ev->destructor(ev);
        }
    }
}
