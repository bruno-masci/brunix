OBJECTS = multiboot_header.o boot.o kmain.o monitor.o io.o libc/string.o libc/stdlib.o
CC = gcc
INCDIR= -I ./ -I ./include -I ./modules -I ./core -I ./arch/x86
CFLAGS = $(INCDIR) -m32 -nostdlib -nostdinc -ffreestanding -fno-builtin -fno-stack-protector \
         -nostartfiles -nodefaultlibs -Wall -Wextra  -c -g -O2 \
         -trigraphs  -fno-exceptions -O0 -Wunused-value
         #-Werror
LDFLAGS = -T linker.ld -m elf_i386 -n -nostdlib
AS = nasm
ASFLAGS = -f elf32

all: kernel.bin

kernel.bin: $(OBJECTS)
	ld $(LDFLAGS) $(OBJECTS) -o kernel.bin
	cp kernel.bin iso/boot/
	grub-mkrescue -o os.iso iso/

run:
	qemu-system-i386 -cdrom os.iso
#	-S -s & gdb  \
#	        -ex 'file kernel.elf' \
 #           -ex 'target remote localhost:1234' \
  #          -ex 'layout src' \
   #         -ex 'layout regs' \
    #        -ex 'layout asm' \
     #       -ex 'break kmain' \
      #      -ex 'continue'

%.o: %.c
	$(CC) $(CFLAGS)  $< -o $@

%.o: %.s
	$(AS) $(ASFLAGS) $< -o $@

clean:
	rm -rf *.o libc/*.o kernel.bin os.iso
	rm -rf iso/boot/kernel.bin
