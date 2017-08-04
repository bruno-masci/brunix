OUTPUT_NAME = brunix
TOPDIR = $(shell pwd)
SUBDIRS = kernel libkern
AS = nasm
ASFLAGS = -f elf32 -g
CROSS_CC = /home/bmasci/opt/cross/bin/i686-elf-gcc
CROSS_OBJCOPY = /home/bmasci/opt/cross/bin/i686-elf-objcopy
CFLAGS = -ffreestanding -c -g -O2 -std=gnu99 -Wall -Wextra -pedantic
LDFLAGS = -T linker.ld -ffreestanding -O2 -nostdlib

default: compile

full: clean $(OUTPUT_NAME).elf run

compile: $(OUTPUT_NAME).elf

$(OUTPUT_NAME).elf:
	@echo Linking kernel into {$(OUTPUT_NAME).elf}
	@$(CROSS_CC) $(LDFLAGS) -o $(OUTPUT_NAME).elf -Wl,-Map,System.map $^ -lgcc
	@$(CROSS_OBJCOPY) --only-keep-debug $(OUTPUT_NAME).elf $(OUTPUT_NAME).sym
	@$(CROSS_OBJCOPY) --strip-debug $(OUTPUT_NAME).elf $(OUTPUT_NAME)-nosym.elf
	@cp $(OUTPUT_NAME)-nosym.elf iso/boot/$(OUTPUT_NAME).elf
	@echo Generating ISO image file...
	@grub-mkrescue -d misc/grub/i386-pc -o os.iso iso/	2>/dev/null # -d is needed for amd64 host platforms

run: run-qemu

run-qemu:
	qemu-system-i386 -cdrom os.iso -m 512M

run-bochs:
	bochs

run-qemu-gdb:
	qemu-system-i386 -cdrom os.iso \
	-S -s & gdb  \
	        -ex 'file $(OUTPUT_NAME).elf' \
            -ex 'target remote localhost:1234' \
            -ex 'layout regs' \
            -ex 'layout asm' \
            -ex 'break kmain'

%.o: %.c
	@echo + cc $<
	@$(CROSS_CC) $(CFLAGS) $< -o $@
	@$(CROSS_CC) -M -MF $<.dep $(CFLAGS) $<

%.o: %.asm
	@echo + as $<
	@$(AS) $(ASFLAGS) $< -o $@

clean:
	@echo Cleaning the rest...
	@rm -rf $(OUTPUT_NAME).elf $(OUTPUT_NAME)-nosym.elf $(OUTPUT_NAME).sym System.map
	@rm -rf os.iso iso/boot/$(OUTPUT_NAME).elf


include $(addsuffix /Makefile,$(SUBDIRS))