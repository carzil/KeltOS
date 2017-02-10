#!/bin/bash
/opt/gnuarmeclipse/qemu/bin/qemu-system-gnuarmeclipse -M STM32-E407 -cpu cortex-m4 -kernel \
kernel.elf -s --verbose --verbose --semihosting-config enable=on,target=native -d \
unimp,guest_errors
