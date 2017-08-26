#ifndef _KELT_DEFS_ASM_H_
#define _KELT_DEFS_ASM_H_


#define FUNCTION(name)      \
    .globl name;            \
    .type name, %function;  \
name

#define ISR_HANDLER(name)   \
    .globl name;            \
    .align 2;               \
name

/* r0 should contain task pointer */
.macro RESTORE_CONTEXT
    /* get stack top */
    ldr     r1, [r0, #0]
    ldmia   r1!, {r4-r11}
    msr     psp, r1
.endm

.macro SAVE_CONTEXT
    /* processor already saved r0, r1, r2, r3, r12,
       pc, lr and sp, save the rest */
    mrs     r1, psp
    stmdb   r1!, {r4-r11}
    str     r1, [r0, #0]
.endm

.syntax unified

#endif
