#ifndef _KELT_DRIVERS_NVIC_H_
#define _KELT_DRIVERS_NVIC_H_

#define NVIC_INT_CTRL_REG       (*((volatile u32*)0xe000ed04))
#define NVIC_PENDSV_SET_BIT     (1 << 28)

#endif
