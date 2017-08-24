MAKEFLAGS   += -rR
ARM_PREFIX  := arm-none-eabi-
CC          := $(ARM_PREFIX)gcc
LD          := $(ARM_PREFIX)ld
AR          := $(ARM_PREFIX)ar
OBJCOPY     := $(ARM_PREFIX)objcopy
INCLUDE_DIR := $(realpath include/)
COMMONFLAGS := $(DEPFLAGS) -Wall -Wextra -nostdlib -nostartfiles -mcpu=cortex-m4 -mthumb -Wa,-mimplicit-it=thumb -g -I$(INCLUDE_DIR)
CCFLAGS     := $(COMMONFLAGS) -std=gnu99 -ffreestanding
ASMFLAGS    := $(COMMONFLAGS)
LDFLAGS     := -T link.ld
SUBSYSTEMS  := sched/ kernel/ drivers/ reactor/
C_SOURCES   := $(shell find $(SUBSYSTEMS) -name "*.c")
ASM_SOURCES := $(shell find $(SUBSYSTEMS) -name "*.S")
C_OBJS      := $(C_SOURCES:.c=.o)
ASM_OBJS    := $(ASM_SOURCES:.S=.o)

%.o: %.c
	@echo -e "    \033[1m\033[34m[CC] \033[37m$<\033[0m"
	@$(CC) -c $(CCFLAGS) -MT $@ -MMD -MP -MF $*.Td $< -o $@
	@mv -f $*.Td $*.d && touch $@

%.o: %.S
	@echo -e "    \033[1m\033[35m[AS] \033[37m$<\033[0m"
	@$(CC) -c $(ASMFLAGS) -MT $@ -MMD -MP -MF $*.Td $< -o $@
	@mv -f $*.Td $*.d && touch $@

kelt.elf: $(C_OBJS) $(ASM_OBJS)
	@echo -e "    \033[1m\033[33m[LD] \033[37m$@\033[0m"
	@$(LD) $(LDFLAGS) $(ASM_OBJS) $(C_OBJS) -o kelt.elf
	@echo -e "    \033[1m\033[32m[OBJCOPY] \033[37mkelt.sym\033[0m"
	@$(OBJCOPY) --only-keep-debug kelt.elf kelt.sym
	@echo -e "    \033[1m\033[32m[OBJCOPY] \033[37mkelt.bin\033[0m"
	@$(OBJCOPY) --strip-debug -Obinary kelt.elf kelt.bin

clean:
	@rm -f $(ASM_OBJS)
	@rm -f $(C_OBJS)
	@rm -f $(ASM_OBJS:.o=.d)
	@rm -f $(C_OBJS:.o=.d)
	@rm -f kelt.elf
	@rm -f kelt.bin
	@rm -f kelt.sym

all: kelt.elf
.PHONY:

%.d: ;
.PRECIOUS: %.d

include $(shell find $(SUBSYSTEMS) -name "*.d")
