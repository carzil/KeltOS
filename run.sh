#!/bin/bash
qemu-system-arm -cpu cortex-m4 -M lm3s811evb -kernel kernel.bin -serial stdio -s -m 128kb

