#pragma once

#include "kernel/defs.h"

#define BUG() panic("bug");
#define BUG_ON(e) if (e) { BUG() }
#define BUG_ON_NULL(e) if (e == NULL) { BUG() }
#define BUG_ON_ZERO(e) if (e == 0) { BUG() }
#define BUG_ON_REACH() BUG()

void panic(const char* msg);
