#ifndef _KELT_DEFS_ASM_H_
#define _KELT_DEFS_ASM_H_

#include "kernel/defs.h"

#define FUNCTION(name) \
    .globl name; \
    .type name, %function; \
name

.syntax unified

#endif
