ARM_PREFIX=arm-none-eabi-
CC=$(ARM_PREFIX)gcc
LD=$(ARM_PREFIX)ld
COMMON_FLAGS=-Wall -Wextra -nostdlib -nostartfiles
CC_FLAGS=$(COMMON_FLAGS) -std=gnu99 -ffreestanding
ASM_FLAGS=$(COMMON_FLAGS)

kernel: printk start irq main
	$(LD) -T link.ld -o kernel.bin printk.o start.o irq.o main.o

start: start.S
	$(CC) -c $(ASM_FLAGS) $< -o $@.o

irq: irq.S
	$(CC) -c $(ASM_FLAGS) $< -o $@.o

printk: printk.c
	$(CC) -c $(CC_FLAGS) $< -o $@.o

main: main.c
	$(CC) -c $(CC_FLAGS) $< -o $@.o
