#pragma once

#include "kernel/defs.h"

#define STR(a) #a
#define XSTR(a) STR(a)

#define BUG_ON_REACH() panic("BUG occured at " __FILE__ ", line " XSTR(__LINE__) "\n");
#define BUG_ON(e) if (e) { BUG_ON_REACH() }
#define BUG_ON_NULL(e) if ((e) == NULL) { BUG_ON_REACH() }
#define BUG_ON_ZERO(e) if ((e) == 0) { BUG_ON_REACH() }

void panic(const char* msg);
