#!/bin/sh
wget "https://developer.arm.com/-/media/Files/downloads/gnu-rm/6-2017q2/gcc-arm-none-eabi-6-2017-q2-update-linux.tar.bz2\?product\=GNU%20ARM%20Embedded%20Toolchain,64-bit,,Linux,6-2017-q2-update" -O gcc.tar.bz2
tar -xf gcc.tar.bz2
make ARM_PREFIX=./gcc-arm-none-eabi-6-2017-q2-update/bin/arm-none-eabi-
