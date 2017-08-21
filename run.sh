#!/bin/bash
/opt/gnuarmeclipse/qemu/bin/qemu-system-gnuarmeclipse \
    -M STM32F4-Discovery \
    -cpu cortex-m4 \
    -kernel kelt.bin \
    -serial stdio \
    --verbose --verbose \
    --semihosting-config enable=on,target=native -d unimp,guest_errors \
    -s \
    $@
