#include "kernel/defs_asm.h"

FUNCTION(sched_return_to):
    bx      lr

FUNCTION(sched_switch_in):
    bx      lr
