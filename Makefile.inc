C_source-$(MODULE) := $(addprefix $(subst _,/,$(MODULE))/,$(filter %.c,$(C_source)))
ASM_source-$(MODULE) := $(addprefix $(subst _,/,$(MODULE))/,$(filter %.S,$(ASM_source)))

OBJS-$(MODULE) := $(C_source-$(MODULE):.c=.o)
OBJS-$(MODULE) += $(ASM_source-$(MODULE):.S=.o)

$(MODULE): $(OBJS-$(MODULE))

$(OUTPUT_NAME).elf: $(OBJS-$(MODULE))

DEPS-$(MODULE) := $(C_source-$(MODULE):.c=.c.dep)

clean: clean-$(MODULE)

clean-$(MODULE): clean-% :
	@echo Cleaning $(subst _,/,$*) module...
	@$(RM) kernel/*.asm libkern/*.asm
	@$(RM) $(OBJS-$*) $(DEPS-$*) $(C_source-$*:.c)