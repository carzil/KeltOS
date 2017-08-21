#include "kernel/panic.h"
#include "kernel/memory.h"
#include "kernel/irq.h"
#include "drivers/semihosting.h"

void panic(const char* msg)
{
    irq_disable_force();
    smhost_print("Panic: ", 7);
    smhost_print(msg, strlen(msg));
    smhost_print("\n", 1);
    for (;;) {
        asm("wfi");
    }
}
