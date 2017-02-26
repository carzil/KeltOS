export MAKEFILE_BUILD_PATH=$(realpath Makefile.build)
export PROJECT_ROOT=$(realpath .)

SUBSYSTEMS=sched kernel bluetooth drivers

all: build

-include $(MAKEFILE_BUILD_PATH)

build: $(ASM_OBJS) $(C_OBJS) $(SUBSYSTEMS)
	@$(LD) $(LD_FLAGS) -T link.ld $(ASM_OBJS) $(C_OBJS) $(SUBSYSTEMS_OBJS) -o kernel.elf
	@echo -e "    \033[1m\033[33m[LD] \033[37mkernel.elf\033[0m"
	@$(OBJCOPY) --only-keep-debug kernel.elf kernel.sym
	@echo -e "    \033[1m\033[32m[OBJCOPY] \033[37mkernel.sym\033[0m"
	@$(OBJCOPY) --strip-debug -Obinary kernel.elf kernel.bin
	@echo -e "    \033[1m\033[32m[OBJCOPY] \033[37mkernel.bin\033[0m"

clean_extra:
	@rm -rf kernel.elf
	@rm -rf kernel.bin
	@rm -rf kernel.sym
