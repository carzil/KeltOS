#include "kernel/args.h"
#include "kernel/printk.h"
#include "kernel/types.h"
#include "kernel/defs.h"
#include "kernel/alloc.h"
#include "kernel/spinlock.h"
#include "sched/sched.h"
#include "drivers/semihosting.h"
#include "kernel/timer.h"

enum { PRINTK_BUF_SIZE = 11 };
static const char digits[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};

static char kring_buffer[RING_BUFFER_SIZE] = { 0 };
static size_t kring_pos = 0;
struct spinlock kring_buffer_lock;

struct task* _dump_task = NULL;

void log_buffer_put(const char* str)
{
    spinlock_lock(&kring_buffer_lock);
    for (; *str != '\0'; str++) {
        kring_buffer[kring_pos] = *str;
        kring_pos = (kring_pos + 1) & (RING_BUFFER_SIZE - 1);
    }
    spinlock_unlock(&kring_buffer_lock);
    // sched_task_wake_up(_dump_task);
}

size_t bprintu32(char* buf, u32 a, int base)
{
    size_t i;
    size_t bytes;

    if (a == 0) {
        buf[0] = '0';
        return 1;
    }
    for (bytes = 0; a > 0; bytes++) {
        buf[bytes] = digits[a % base];
        a /= base;
    }
    for (i = 0; i < bytes / 2; i++) {
        buf[i] ^= buf[bytes - i - 1];
        buf[bytes - i - 1] ^= buf[i];
        buf[i] ^= buf[bytes - i - 1];
    }
    return bytes;
}

size_t bprints32(char* buf, s32 a, int base)
{
    if (a < 0) {
        *(buf++) = '-';
        return 1 + bprintu32(buf, -a, base);
    }
    return bprintu32(buf, a, base);
}

size_t bprintstr(char* buf, const char* str)
{
    const char* cur = str;
    size_t length = 0;
    while (*cur) {
        buf[length++] = *(cur++);
    }
    return length;
}

size_t bprintptr(char* buf, void* ptr)
{
    size_t bytes;
    if (ptr == NULL) {
        return bprintstr(buf, "(nil)");
    }
    bytes = bprintstr(buf, "0x");
    return bytes + bprintu32(buf + bytes, (u32) ptr, 16);
}

void printk(const char* fmt, ...)
{
    const char* cursor = fmt;
    int idle = 1;
    va_list args;
    u32 u32value;
    s32 s32value;
    void* ptrvalue;
    char* str;
    char buf[PRINTK_BUF_SIZE];
    size_t size;

    va_start(args, fmt);
    while (*cursor) {
        if (idle) {
            if (*cursor != '%') {
                smhost_putc(*cursor);
            } else {
                idle = 0;
            }
        } else {
            switch (*cursor) {
            case 'u':
                u32value = va_arg(args, u32);
                size = bprintu32(buf, u32value, 10);
                break;
            case 'x':
                u32value = va_arg(args, u32);
                size = bprintu32(buf, u32value, 16);
                break;
            case 'd':
                s32value = va_arg(args, s32);
                size = bprints32(buf, s32value, 10);
                break;
            case 'p':
                ptrvalue = va_arg(args, void*);
                size = bprintptr(buf, ptrvalue);
                break;
            case 's':
                str = va_arg(args, char*);
                smhost_printz(str);
                size = 0;
                break;
            default:
                size = 0;
                break;
            }
            if (size) {
                smhost_print(buf, size);
            }
            idle = 1;
        }
        ++cursor;
    }
    va_end(args);
}

void dump_kernel_log_task()
{
    size_t kring_pos_last = 0;
    for (;;) {
        spinlock_lock(&kring_buffer_lock);
        if (kring_pos_last != kring_pos) {
            if (kring_pos < kring_pos_last) {
                /* buffer overflow happened */
                smhost_print(kring_buffer + kring_pos_last, RING_BUFFER_SIZE - kring_pos_last);
                smhost_print(kring_buffer, kring_pos);
            } else {
                smhost_print(kring_buffer + kring_pos_last, kring_pos - kring_pos_last);
            }
            kring_pos_last = kring_pos;
        }
        spinlock_unlock(&kring_buffer_lock);
        sched_task_set_sleeping(_dump_task, TASK_SLEEPING);
        asm (
            "mov    r0, #1\n"
            "swi    #0"
        );
    }
}

void printk_init()
{
    // _dump_task = sched_start_task(&dump_kernel_log_task, PRIORITY_HIGH);
    // _dump_task->name = "kernel_dump_log";
}
