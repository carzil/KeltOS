#ifndef _KELT_KERNEL_SPINLOCK_H_
#define _KELT_KERNEL_SPINLOCK_H_

#include "kernel/types.h"

struct spinlock {
    u32 lock;
};

void spinlock_lock(struct spinlock* lock);
void spinlock_unlock(struct spinlock* lock);

#endif
