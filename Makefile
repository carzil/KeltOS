-include Makefile.vars

build:
	make -C sched
	make -C kernel
	$(CC) -c $(ASM_FLAGS) start.S -o start.o
	$(CC) -c $(CC_FLAGS) main.c -o main.o
	$(LD) -T link.ld -o kernel.bin sched/sched.o kernel/kernel.o start.o main.o
	$(OBJCOPY) --only-keep-debug kernel.bin kernel.sym
	$(OBJCOPY) --strip-debug kernel.bin
