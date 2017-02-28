#include "drivers/semihosting.h"
#include "kernel/irq.h"

int smhost_stdout;

s32 smhost_gateway(u32 op, void* blk)
{
    int res;
    asm (
        "mov    r0, %[op]\n"
        "mov    r1, %[blk]\n"
        "bkpt   0xab\n"
        "mov    %[res], r0"
        : [res]"=r"(res)
        : [op]"r"(op), [blk]"r"(blk)
        : "r0", "r1"
    );
    return res;
}

u32 smhost_open(const char* fname, u32 fname_size, u32 mode)
{
    u32 smhost_req[3] = {
        (u32)fname,
        mode,
        fname_size
    };
    return smhost_gateway(SMHOST_OPEN, smhost_req);
}

u32 smhost_print(void* buf, u32 size)
{
    u32 smhost_req[3] = {
        smhost_stdout,
        (u32)buf,
        size
    };
    return smhost_gateway(SMHOST_WRITE, smhost_req);
}

u32 smhost_printz(const char* buf)
{
    irq_disable();
    s32 res = smhost_gateway(SMHOST_WRITE0, (void*)buf);
    irq_enable();
    return res;
}
