OUTPUT_NAME = brunix
TOPDIR = $(shell pwd)
SUBDIRS = kernel libkern
AS = /home/bmasci/opt/cross/bin/i686-elf-gcc
ASFLAGS = -ffreestanding -c -g -O2 -std=gnu99
CROSS_CC = /home/bmasci/opt/cross/bin/i686-elf-gcc
CROSS_OBJCOPY = /home/bmasci/opt/cross/bin/i686-elf-objcopy
CFLAGS = -ffreestanding -c -g -O2 -std=gnu99 -Wall -Wextra -pedantic
CFLAGS_ASM = -ffreestanding
LDFLAGS = -T linker.ld -ffreestanding -O2 -nostdlib
QEMU = qemu-system-i386
QEMU_OPTS = -cdrom os.iso -m 512M -serial mon:stdio -gdb tcp\:\:26000


default: compile

full: clean $(OUTPUT_NAME).elf run

compile: $(OUTPUT_NAME).elf

$(OUTPUT_NAME).elf:
	@echo Linking kernel into $(OUTPUT_NAME).elf file...
	@$(CROSS_CC) $(LDFLAGS) -o $(OUTPUT_NAME).elf -Wl,-Map,System.map $^ -lgcc
	@$(CROSS_OBJCOPY) --only-keep-debug $(OUTPUT_NAME).elf $(OUTPUT_NAME).sym
	@$(CROSS_OBJCOPY) --strip-debug $(OUTPUT_NAME).elf $(OUTPUT_NAME)-nosym.elf
	@cp $(OUTPUT_NAME)-nosym.elf iso/boot/$(OUTPUT_NAME).elf
	@echo Generating ISO image file...
	@grub-mkrescue -d misc/grub/i386-pc -o os.iso iso/ 2>/dev/null # -d is needed for amd64 host platforms

run: run-qemu

gdb:
	gdb -n -x .gdbinit

run-qemu:
	$(QEMU) $(QEMU_OPTS)

run-bochs:
	bochs

run-qemu-gdb:
	@echo "***"
	@echo "*** Now run 'make gdb'." 1>&2
	@echo "***"
	$(QEMU) $(QEMU_OPTS) -S

%.o: %.c
	@echo + cc $<
	@$(CROSS_CC) $(CFLAGS) -isystem $(TOPDIR)/include/ $< -o $@
	@$(CROSS_CC) $(CFLAGS_ASM) -isystem $(TOPDIR)/include/ -S $< -o $<.asm
	@$(CROSS_CC) -isystem $(TOPDIR)/include/ -M -MF $<.dep $(CFLAGS) $<

%.o: %.S
	@echo + as $<
	@$(AS) $(ASFLAGS) -isystem $(TOPDIR)/include/ $< -o $@

clean:
	@echo Cleaning the rest...
	@rm -rf $(OUTPUT_NAME).elf $(OUTPUT_NAME)-nosym.elf $(OUTPUT_NAME).sym System.map
	@rm -rf os.iso iso/boot/$(OUTPUT_NAME).elf


include $(addsuffix /Makefile,$(SUBDIRS))