# What is KeltOS?
KeltOS is very simple ARM realtime operating system.

# How to build it?
You need make, bare-metal gcc compiler (`arm-none-eabi-gcc` in Arch Linux) and maybe gdb.
Run:
```
make
```
And KeltOS will be built.

# How can I ran it?
Currently, KeltOS supports only IntegratorCP board and not tested on real hardware. We use qemu 
to run it:
```
qemu-system-arm -M integratorcp -m 128 -kernel kernel.bin -serial stdio
```
Also, if you want to debug it, run with `-s -S` flags. QEMU will listen at :1234 for gdb and when 
it connects starts the kernel.

