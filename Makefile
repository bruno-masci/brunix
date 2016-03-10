ARCH = x86
NAME = brunix
OBJECTS = multiboot_header.o boot.o kmain.o arch/x86/kernel/vga.o io.o descriptor_tables.o gdt.o interrupt.o isr.o printk.o libc/string.o libc/stdlib.o
CC = gcc
INCDIR= -I ./ -I ./include -I ./arch/x86/include
CFLAGS = $(INCDIR) -m32 -nostdlib -nostdinc -ffreestanding -fno-builtin -fno-stack-protector \
         -nostartfiles -nodefaultlibs -Wall -Wextra  -c -g -O2 \
         -trigraphs  -fno-exceptions -O0 -Wunused-value -std=gnu99
         #-Werror
LDFLAGS = -T linker.ld -m elf_i386 -n -nostdlib --defsym __BUILD_DATE=$(shell date +'%Y%m%d') --defsym __BUILD_TIME=$(shell date +'%H%M%S')
AS = nasm
ASFLAGS = -f elf32 -g

all: $(NAME).elf

$(NAME).elf: $(OBJECTS)
	ld $(LDFLAGS) $(OBJECTS) -o $(NAME).elf
	cp $(NAME).elf iso/boot/
	grub-mkrescue -d misc/grub/i386-pc -o os.iso iso/	# -d is needed for amd64 host platforms

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
	rm -rf *.o arch/x86/kernel/*.o libc/*.o $(NAME).elf os.iso
	rm -rf iso/boot/$(NAME).elf


#revisar si poner estas opciones a gcc: -Wpointer-arith -Wcast-align -Wno-unused-parameter -fno-rtti
