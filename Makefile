export MAKEFILE_BUILD_PATH=$(realpath Makefile.build)
export PROJECT_ROOT=$(realpath .)

SUBSYSTEMS=sched kernel

all: build

-include $(MAKEFILE_BUILD_PATH)

build: $(ASM_OBJS) $(C_OBJS) $(SUBSYSTEMS)
	@$(LD) -T link.ld $(ASM_OBJS) $(C_OBJS) $(SUBSYSTEMS_OBJS) -o kernel.elf
	@$(OBJCOPY) --only-keep-debug kernel.elf kernel.sym
	@$(OBJCOPY) --strip-debug -Obinary kernel.elf kernel.bin

clean_extra:
	rm -rf kernel.elf
	rm -rf kernel.bin
	rm -rf kernel.sym

