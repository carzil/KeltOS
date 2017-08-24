#include "kernel/types.h"
#include "kernel/panic.h"
#include "kernel/timer.h"
#include "drivers/systick.h"
#include "drivers/semihosting.h"
#include "sched/sched.h"
#include "kernel/irq.h"
#include "sched/cpu.h"

void NAKED systick_handler()
{
    asm (
        "push   {lr}\n"
        "bl     timer_tick\n"
        "pop    {lr}\n"
    );
    EXCEPTION_RETURN_DEFAULT();
}

void systick_disable()
{
    *(volatile u32*)SYSTICK_CSR &= ~(u32)(SYSTICK_ENABLE | SYSTICK_INT);
}

void systick_enable()
{
    *(volatile u32*)SYSTICK_CSR = SYSTICK_ENABLE | SYSTICK_INT | 0x4;
}

void systick_init()
{
    /*
     * By default SysTick operates on the same frequency as the processor.
     * Don't forget to subtract 1, cause in [0, n-1] there are n numbers.
     */
    *(volatile u32*)SYSTICK_RVR = (CPU_CLOCK_RATE_HZ / TICK_RATE_HZ) - 1;

    /* Enable SysTick and its interrupts */
    systick_enable();

    if (!(*(volatile u32*)SYSTICK_CSR & SYSTICK_ENABLE)) {
        panic("cannot enable SysTick");
    }
}
