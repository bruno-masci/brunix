TOPDIR = $(shell pwd)
ARCH = x86
NAME = brunix
KERNDIRS = kernel arch/$(ARCH)/kernel libkern
SUBDIRS = $(KERNDIRS)
CC = gcc
INCDIR= -I ./ -I ./include -I ./arch/x86/include
CFLAGS = $(INCDIR) -m32 -nostdlib -nostdinc -ffreestanding -fno-builtin -fno-stack-protector \
         -nostartfiles -nodefaultlibs -c -g -O2 \
         -trigraphs -fno-exceptions -O0 -std=gnu99 -w
         #-Werror -Wall -Wextra -Wunused-value
LDFLAGS = -T linker.ld -m elf_i386 -n -nostdlib --defsym __BUILD_DATE=$(shell date +'%Y%m%d') --defsym __BUILD_TIME=$(shell date +'%H%M%S')
AS = nasm
ASFLAGS = -f elf32 -g

default: all

all: $(NAME).elf

$(NAME).elf:
	ld $(LDFLAGS) -o $(NAME).elf $^
	objcopy --only-keep-debug $(NAME).elf $(NAME).sym
	objcopy --strip-debug $(NAME).elf $(NAME)-nosym.elf
	cp $(NAME)-nosym.elf iso/boot/$(NAME).elf
	grub-mkrescue -d misc/grub/i386-pc -o os.iso iso/	# -d is needed for amd64 host platforms


run:
	qemu-system-i386 -cdrom os.iso
	     
run-bochs:
	bochs
	
run-qemu-gdb:
	qemu-system-i386 -cdrom os.iso \
	-S -s & gdb  \
	        -ex 'file brunix.elf' \
            -ex 'target remote localhost:1234' \
            -ex 'layout regs' \
            -ex 'layout asm' \
            -ex 'break kmain'

%.o: %.c
	$(CC) $(CFLAGS)  $< -o $@

%.o: %.asm
	$(AS) $(ASFLAGS) $< -o $@

clean:
	rm -rf $(NAME).elf $(NAME)-nosym.elf $(NAME).sym os.iso
	rm -rf iso/boot/$(NAME).elf


#revisar si poner estas opciones a gcc: -Wpointer-arith -Wcast-align -Wno-unused-parameter -fno-rtti

include $(addsuffix /Makefile,$(SUBDIRS))
