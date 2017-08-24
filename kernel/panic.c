#include "kernel/panic.h"
#include "kernel/memory.h"
#include "kernel/irq.h"
#include "drivers/semihosting.h"
#include "drivers/systick.h"

void panic(const char* msg)
{
    systick_disable();
    irq_disable_force();
    smhost_print("Panic: ", 7);
    smhost_print(msg, strlen(msg));
    smhost_print("\n", 1);
    for (;;) {
        asm("wfi");
    }
}
