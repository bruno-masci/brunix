# Run 'make V=1' to turn on verbose commands, or 'make V=0' to turn them off.
ifeq ($(V),1)
override V =
endif
ifeq ($(V),0)
override V = @
endif

-include conf/env.mk

# try to generate a unique GDB port
GDBPORT	:= $(shell expr `id -u` % 5000 + 25000)


CROSS_COMPILER_PATH = /home/bmasci/opt/cross/bin
AS =      $(CROSS_COMPILER_PATH)/i686-elf-gcc
CC =      $(CROSS_COMPILER_PATH)/i686-elf-gcc
OBJCOPY = $(CROSS_COMPILER_PATH)/i686-elf-objcopy
NM =      $(CROSS_COMPILER_PATH)/i686-elf-nm

## Compiler flags
# Do not optimise for now with "-O1" and so on, so we can do some validations.
CFLAGS = -ffreestanding -c -g -std=gnu99 -Wall -Wextra -pedantic

# Add -fno-stack-protector if the option exists.
CFLAGS += $(shell $(CC) -fno-stack-protector -E -x c /dev/null >/dev/null 2>&1 && echo -fno-stack-protector)

CPPFLAGS = $(CFLAGS) -E -x c

## Assembler flags
ASFLAGS = $(CFLAGS)

# Common linker flags
LDFLAGS = -T linker.ld -ffreestanding -nostdlib

QEMU = qemu-system-i386
QEMU_OPTS = -cdrom os.iso -m 512M -serial mon:stdio -gdb tcp::$(GDBPORT)
IMAGES = $(OUTPUT_NAME).elf

OUTPUT_NAME = brunix
TOPDIR = $(shell pwd)
SUBDIRS = kernel libkern

default: compile

full: clean $(OUTPUT_NAME).elf qemu

compile: linker.ld $(OUTPUT_NAME).elf

$(OUTPUT_NAME).elf:
	@echo Linking kernel into $(OUTPUT_NAME).elf file...
	$(V)$(CC) $(LDFLAGS) -o $(OUTPUT_NAME).elf -Wl,-Map,System.map $^ -lgcc
	@echo Validating multiboot image...
	$(V)./multiboot-checker.sh $(OUTPUT_NAME).elf
	@cp $(OUTPUT_NAME).elf iso/boot/$(OUTPUT_NAME).elf
	@echo Generating ISO image file...
	$(V)grub-mkrescue -d misc/grub/i386-pc -o os.iso iso/ 2>/dev/null

#@$(CROSS_OBJCOPY) --only-keep-debug $(OUTPUT_NAME).elf $(OUTPUT_NAME).sym
#@$(CROSS_OBJCOPY) --strip-debug $(OUTPUT_NAME).elf $(OUTPUT_NAME)-nosym.elf

linker.ld:
	@echo Preprocessing linker script
	$(CC) $(CPPFLAGS) -isystem $(TOPDIR)/include/ linker.ld.pp | grep -v '^#' > linker.ld

.gdbinit: .gdbinit.tmpl
	sed "s/localhost:1234/localhost:$(GDBPORT)/" < $^ > $@

gdb:
	$(GDB)

pre-qemu: .gdbinit

qemu: compile pre-qemu
	$(QEMU) $(QEMU_OPTS)

qemu-nox: $(IMAGES) pre-qemu
	@echo "***"
	@echo "*** Use Ctrl-a x to exit qemu"
	@echo "***"
	$(QEMU) -nographic $(QEMU_OPTS)

qemu-gdb: $(IMAGES) pre-qemu
	@echo "***"
	@echo "*** Now run 'make gdb'." 1>&2
	@echo "***"
	$(QEMU) $(QEMUOPTS) -S

qemu-nox-gdb: $(IMAGES) pre-qemu
	@echo "***"
	@echo "*** Now run 'make gdb'." 1>&2
	@echo "***"
	$(QEMU) -nographic $(QEMU_OPTS) -S







print-gdbport:
	@echo $(GDBPORT)


%.o: %.c
	@echo + cc $<
	$(V)$(CC) $(CFLAGS) -isystem $(TOPDIR)/include/ $< -o $@
	$(V)$(CC) $(ASFLAGS) -isystem $(TOPDIR)/include/ -S $< -o $<.asm
	$(V)$(CC) -isystem $(TOPDIR)/include/ -M -MF $<.dep $(CFLAGS) $<

%.o: %.S
	@echo + as $<
	$(V)$(AS) $(ASFLAGS) -isystem $(TOPDIR)/include/ $< -o $@

clean:
	@echo Cleaning the rest...
	$(V)rm -rf $(OUTPUT_NAME).elf $(OUTPUT_NAME)-nosym.elf $(OUTPUT_NAME).sym $(OUTPUT_NAME).nm-sym System.map linker.ld
	$(V)rm -rf os.iso iso/boot/$(OUTPUT_NAME).elf


# Include Makefrags for subdirectories
include $(addsuffix /Makefile,$(SUBDIRS))