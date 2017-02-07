build:
	$(CC) -c $(CC_FLAGS) printk.c -o printk.o
	$(CC) -c $(CC_FLAGS) irq.c -o irq.o
	$(CC) -c $(CC_FLAGS) irq.S -o irq_asm.o
	$(LD) -r $(LD_FLAGS) printk.o irq_asm.o irq.o -o kernel.o
