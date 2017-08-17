# What is KeltOS? [![Build Status](https://travis-ci.org/carzil/KeltOS.svg?branch=master)](https://travis-ci.org/carzil/KeltOS)
KeltOS is very simple ARM realtime operating system.

# How to build it?
You need make, bare-metal gcc compiler (`arm-none-eabi-gcc` in Arch Linux) and maybe gdb.
Run:
```
make
```
And KeltOS will be built.

# How can I run it?
Currently, KeltOS supports only Cortex-M4 processor and STM32F407 board and was not tested on real hardware. We use awesome QEMU fork called GNU ARM Eclipse QEMU to run it:
```
/opt/gnuarmeclipse/qemu/bin/qemu-system-gnuarmeclipse -M STM32-E407 -cpu cortex-m4 -kernel kernel.elf --semihosting-config enable=on,target=native -d unimp,guest_errors
```
Or simply:
```
./run.sh
```
Also, if you want to debug it, run with `-s -S` flags. QEMU will listen at :1234 for gdb and will start the kernel after successful connection.
