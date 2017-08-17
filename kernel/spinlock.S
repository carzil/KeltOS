#include "kernel/defs_asm.h"

#define LOCKED      0x1
#define NOT_LOCKED  0x0

FUNCTION(spinlock_lock):
    mov     r1, #LOCKED
.try_lock:
    @ r0 contains pointer to spinlock
    ldrex   r2, [r0]
    cmp     r2, #NOT_LOCKED
    strexeq r2, r1, [r0]
    tsteq   r2, r2
    bne     .try_lock
    bx      lr

FUNCTION(spinlock_unlock):
    mov     r1, #NOT_LOCKED
    str     r1, [r0]
    bx      lr
