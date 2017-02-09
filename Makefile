-include Makefile.vars

build:
	make -C sched
	make -C kernel
	$(CC) -c $(ASM_FLAGS) start.S -o start.o
	$(CC) -c $(CC_FLAGS) main.c -o main.o
	$(LD) -T link.ld start.o sched/sched.o kernel/kernel.o main.o -o kernel.elf
	$(OBJCOPY) --only-keep-debug kernel.elf kernel.sym
	$(OBJCOPY) --strip-debug -Obinary kernel.elf kernel.bin
