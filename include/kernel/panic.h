#pragma once

#include "kernel/defs.h"

#define BUG() panic("bug");
#define BUG_ON(e) if (e) { BUG() }
/* TBD
#define BUG_ON_NULL(e)
#define BUG_ON_ZERO(e)
#define BUG_ON_REACH()
*/

void panic(const char* msg);
