#include "kernel/defs_asm.h"

FUNCTION(swi_handler):
    mrs     r0, psp
    bl      syscall_handler
