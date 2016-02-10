OBJECTS = multiboot_header.o boot.o kmain.o common.o monitor.o #io.o
CC = gcc
INCDIR= -I ./ -I ./modules -I ./core -I ./arch/x86
CFLAGS = $(INCDIR) -m32 -nostdlib -nostdinc -ffreestanding -fno-builtin -fno-stack-protector \
         -nostartfiles -nodefaultlibs -Wall -Wextra  -c -g -O2 \
         -trigraphs  -fno-exceptions -O0 -m64 -Wunused-value
         #-Werror
LDFLAGS = -T linker.ld -m elf_x86_64 -n -nostdlib
AS = nasm
ASFLAGS = -f elf64

all: kernel.bin

kernel.bin: $(OBJECTS)
	ld $(LDFLAGS) $(OBJECTS) -o kernel.bin
	cp kernel.bin iso/boot/
	grub-mkrescue -o os.iso iso/

run:
	qemu-system-i386 -cdrom os.iso

%.o: %.c
	$(CC) $(CFLAGS)  $< -o $@

%.o: %.s
	$(AS) $(ASFLAGS) $< -o $@

clean:
	rm -rf *.o kernel.bin os.iso
	rm -rf iso/boot/kernel.bin
