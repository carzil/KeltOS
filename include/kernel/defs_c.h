#pragma once

#define _STRINGIFY(x) #x
#define STRINGIFY(x) _STRINGIFY(x)

#define KELT_READONLY __attribute__((section(".rodata")))
