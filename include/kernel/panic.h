#pragma once

#include "kernel/defs.h"

#define BUG_ON_REACH() printk("BUG occured at %s, line %d", __FILE__, __LINE__); panic("bug");
#define BUG_ON(e) if (e) { BUG_ON_REACH() }
#define BUG_ON_NULL(e) if (e == NULL) { BUG_ON_REACH() }
#define BUG_ON_ZERO(e) if (e == 0) { BUG_ON_REACH() }

void panic(const char* msg);
