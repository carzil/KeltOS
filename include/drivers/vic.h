#ifndef _KELT_DRIVERS_VIC_H_
#define _KELT_DRIVERS_VIC_H_

#include "kernel/types.h"

#define VIC_BASE_ADDR       0x10140000
#define VIC_INTSELECT_REG   (VIC_BASE_ADDR + 0xc)
#define VIC_INTENABLE_REG   (VIC_BASE_ADDR + 0x10)

#define VIC_IRQ             0x0
#define VIC_FIQ             0x1

#define VIC_MAX_IRQ         16

struct _hw_vic {
    u32 irq_status_reg;
    u32 fiq_status_reg;
    u32 raw_int_reg;
    u32 int_select_reg;
    u32 int_enable_reg;
    u32 int_enclear_reg;
    u32 soft_int_reg;
    u32 soft_int_clear_reg;
    u32 protection_reg;
    u32 addr_reg;
    u32 vect_addr;
    u32 vect_addr_reg[16];
    u32 vect_ctl_reg[16];
};

static struct _hw_vic* vic = (struct _hw_vic*)VIC_BASE_ADDR;

static inline void vic_mask_interrupt(u32 idx)
{
    vic->int_enable_reg &= ~(1u << idx);
}

static inline void vic_unmask_interrupt(u32 idx)
{
    vic->int_enable_reg = 1u << idx;
}

static inline void vic_set_fiq(u32 idx)
{
    vic->int_select_reg = 1u << idx;
}

static inline void vic_set_irq(u32 idx)
{
    vic->int_select_reg &= ~(1u << idx);
}

static inline int vic_is_active(u32 idx)
{
    return vic->irq_status_reg & (1u << idx);
}

#define vic_for_each_active(idx) for (idx = 0; idx < VIC_MAX_IRQ; idx++) if (vic->irq_status_reg & (1u << idx))

#endif
