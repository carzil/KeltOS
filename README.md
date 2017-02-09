# What is KeltOS?
KeltOS is very simple ARM realtime operating system.

# How to build it?
You need make, bare-metal gcc compiler (`arm-none-eabi-gcc` in Arch Linux) and maybe gdb.
Run:
```
make
```
And KeltOS will be built.

# How can I run it?
Currently, KeltOS supports only Cortex-M4 processor and lm3s811evb board and was not tested on real hardware. We use qemu 
to run it:
```
qemu-system-arm -cpu cortex-m4 -M lm3s811evb -kernel kernel.bin -serial stdio -s -m 128kb
```
Or simply:
```
./run.sh
```
Also, if you want to debug it, run with `-s -S` flags. QEMU will listen at :1234 for gdb and will start the kernel after successful connection.

