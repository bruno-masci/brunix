







## File structure

 * |── brunix.elf
 * |── brunix-nosym.elf
 * |── brunix.sym
 * |── include
   * |── **arch/x86**: x86 architecture-dependent header files.
   * |── **brunix**: architecture-independent header files.
 * |── **kernel**: kernel source code.
 * |── **libkern**: custom libc (see https://wiki.osdev.org/C_Library) for the kernel.
 * |── Makefile
 * |── Makefile.inc
 * |── **linker.ld**: linker script.
 * |── os.iso
 * |── System.map

# brunix
## Small Unix-like 32-bits x86 OS for fun and learning (stage 0)

Before we start, please note that:
* the information here is complemented with that contained in the source code,
* with x86 we mean 386+ architecture.

### Pre-requisites
* mtools
* xorriso
* grub
* gas
* A <b>proper</b> [cross-compiler](http://wiki.osdev.org/GCC_Cross-Compiler)

### Goals

In this very first stage we are going to outline and depict the project structure. The idea is to do incremental
developments (stage0, stage1, ...) to tackle all the complexities in an easier way.

For now, we'll just create a bare OS (if we even can call it that way) that includes:
* kernel loading (boot),
* basic video management.

We'll use [ELF](http://wiki.osdev.org/ELF) as the kernel image format, [ld](http://wiki.osdev.org/LD) linker from the
cross-compiler just built (see "[Pre-requisites](#Pre-requisites)" section above) to produce an ELF-formatted kernel image,
and [GRUB](https://wiki.osdev.org/GRUB) [bootloader](https://wiki.osdev.org/Bootloader) for booting the kernel.

#### Why ELF?

ELF is a very flexible, well supported and documented file format, that distinguish between TEXT, DATA and BSS sections.
Besides, ELF supports separate load and execution addresses, vital for a
[Higher Half Kernel](https://wiki.osdev.org/Higher_Half_Kernel) (more on this later).

#### Why GRUB?

GRUB, which adheres to the [Multiboot](http://wiki.osdev.org/Multiboot) specification, is very powerful and natively
supports loading ELF files.\
GRUB save us from all the pain of switching from [Real Mode](http://wiki.osdev.org/Real_Mode) to
[Protected Mode](http://wiki.osdev.org/Protected_Mode), as it handles all the unpleasant details. Also, GRUB helps us
avoid having to call [BIOS](https://wiki.osdev.org/BIOS) services.

## The full picture

(please read the questions below in order)

### How do we run our OS?

When we run <i>"make run"</i> (see "Makefile" file) from the project's top level directory, QEMU emulator runs:

	qemu-system-i386 -cdrom os.iso -m 512M

simulating that the OS image (os.iso, an ISO file) is inserted in the CD-ROM drive of a machine with 512M of RAM memory.

### How is the os.iso file created?

When we run <i>"make"</i> or <i>"make compile"</i> (see "Makefile" file) from the project's top level directory, once
"brunix.elf" file gets created and put into "iso" directory, an ISO image is constructed with:

	grub-mkrescue -d misc/grub/i386-pc -o os.iso iso/

IMPORTANT: If you are working on an amd64 platform (as in my case) you need to provide the "-d" option to the command above.

### How is the ELF kernel image generated?

When we run <i>"make"</i> or <i>"make compile"</i> (see "Makefile" file) from the project's top level directory,
prior creation of the ISO file, all source code is compiled (using the GCC cross-compiler for C and
[gas](http://wiki.osdev.org/GAS) for ASM) into [relocatable ELF object files](http://wiki.osdev.org/Object_Files) that
are linked together using ld (really using GCC as a linker) into a conclusive statically linked executable ELF file:

    $(CROSS_CC) $(LDFLAGS) -o $(OUTPUT_NAME).elf -Wl,-Map,System.map $^ -lgcc

The "-Map,System.map" option creates a file called "System.map" containing all the symbols from the ELF image.\
For the "-lgcc" library inclusion, see [Libgcc](https://wiki.osdev.org/Libgcc).

## Now, what really happen after the emulator starts running?

When the computer is turned on (see [Initialization](http://wiki.osdev.org/System_Initialization_(x86))), the CPU starts
in the so called Real Mode for compatibility reasons. In order to get all the power from an x86 CPU, we need to enable
the so called Protected Mode.\
So far, we have an ISO file containing both GRUB code (for booting) and the ELF kernel image.\
Once system initialization has been concluded, the ISO file is read from the CD-ROM drive and GRUB code gets executed.\
GRUB knows where the kernel is in the storage (see "iso/boot/grub/grub.cfg" file) and where to load it in physical RAM
memory for execution - 1 MiB, in this case - (see "linker.ld" file).
Just before jumping to the kernel, GRUB leaves the CPU in Protected Mode with a full 4 GiB addressing space (32 bits),
and [Paging](https://wiki.osdev.org/Paging) and [Interrupts](https://wiki.osdev.org/Interrupts) disabled (more on this later).

## How does GRUB load the ELF-formatted kernel?

In order for GRUB to get the kernel loaded, it needs to know where in RAM
memory we want the kernel to be loaded, which is the entry point of the kernel,
and so on. It gets this information from the ELF headers contained in the
image. And the image
is created by the ld linker. (see linker.ld file)

## How does the kernel start running?
First thing first... our kernel image is ELF formatted and its inner structure is given by the ld linker directives and commands declared in the linker.ld file (see [Linker Scripts](http://wiki.osdev.org/Linker_Scripts)).
When GRUB (or any Multiboot-compliant bootloader, for that matter) loads our kernel image, it needs to check whether the kernel is Multiboot-compliant looking for certain values to be stored at the beginning of the kernel image; that's why we have a "multiboot_header" section (see [multiboot_entry_point.S](/kernel/multiboot_entry_point.S)) at the very first position in the linker.ld file.
Once GRUB has checked the image, it transfers the control to the kernel executing the code at the "_start" symbol.


## NO STANDARD LIBRARY

In the kernel ... .. There's no standard library..!


## MULTIBOOT SPEC / GRUB

Aside,
EBX contains a pointer to the Multiboot information structure
EAX contains the value 0x2BADB002
Note that GRUB configures a stack but we can't trust its location, so we need to define ours.



TODO vEr esto: Since we haven't set up virtual memory yet, all virtual addresses are identical to the physical ones.

detalles sobre GCC/GAS:
- Agregar lo de #ifndef __ASSEMBLER__ para headers compartidos con assembler!
- __attribute__((packed))
- __attribute__ ((noreturn))
        nombrar que no uso __atrib noreturn para que el codigo no cambie y poder debuggear

#include <stddef.h>     // for size_t
#include <stdint.h> // for uint8_t, uint32_t, uintptr_t, etc.
stdbool
y stdarg // for va_list





References:

* https://css.csail.mit.edu/6.858/2014/readings/i386.pdf
* https://wiki.osdev.org/Serial_Ports
* http://www.cse.iitd.ernet.in/os-lectures
* https://wiki.osdev.org
* https://wiki.osdev.org/Bare_Bones
* http://wiki.osdev.org/GCC_Cross-Compiler
* http://wiki.osdev.org/Why_do_I_need_a_Cross_Compiler%3F
* http://wiki.osdev.org/LD
* https://wiki.osdev.org/System_V_ABI (en duda)
* https://wiki.osdev.org/Calling_Conventions (en duda)
* http://wiki.osdev.org/System_Initialization_(x86)
* http://wiki.osdev.org/Memory_Map_(x86)
* //https://wiki.osdev.org/Interrupts
* //https://wiki.osdev.org/Non_Maskable_Interrupt
* http://wiki.osdev.org/Real_Mode
* http://wiki.osdev.org/Protected_Mode
* //https://wiki.osdev.org/Global_Descriptor_Table
* //https://wiki.osdev.org/Paging
* http://wiki.osdev.org/Printing_To_Screen
* http://www.brokenthorn.com/Resources/OSDevVga.html
* http://wiki.osdev.org/Multiboot
* https://www.gnu.org/software/grub/manual/multiboot/multiboot.html
* http://www.jamesmolloy.co.uk/tutorial_html
* http://www.osdever.net/bkerndev/index.php
* http://os.phil-opp.com/multiboot-kernel.html
* https://wiki.osdev.org/Inline_Assembly
* https://en.wikibooks.org/wiki/The_Linux_Kernel/System


https://pdos.csail.mit.edu/6.828/2014/readings/elf.pdf
https://pdos.csail.mit.edu/6.828/2014/readings/pcasm-book.pdf

https://gcc.gnu.org/onlinedocs/gcc-3.3/gcc/C-Extensions.html