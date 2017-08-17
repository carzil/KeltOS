#ifndef _KELT_KERNEL_SPINLOCK_H_
#define _KELT_KERNEL_SPINLOCK_H_

#include "kernel/types.h"
#include "kernel/irq.h"

struct spinlock {
    u32 lock;
};

void spinlock_lock(struct spinlock* lock);
void spinlock_unlock(struct spinlock* lock);

static inline void spinlock_lock_irq(struct spinlock* lock, int* flags)
{
    if (irq_enabled()) {
        *flags = 1;
        disable_irq();
    } else {
        *flags = 0;
    }
    spinlock_lock(lock);
}

static inline void spinlock_unlock_irq(struct spinlock* lock, int* flags)
{
    if (*flags) {
        enable_irq();
    }
    spinlock_unlock(lock);
}

#endif
