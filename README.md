# brunix
## Small Unix-like 32-bits x86 OS for fun and learning (stage 0)

Before we start, please note that:
* the information here is complemented with that contained in the source code,
* with x86 we mean 386+ architecture.

## Pre-requisites
* mtools
* xorriso
* grub
* nasm
* A <b>proper</b> [cross-compiler](http://wiki.osdev.org/GCC_Cross-Compiler)

## Goals

In this very first stage we are going to outline and depict the project
structure. The idea is to do incremental developments (stage0, stage1, ...) to
tackle all the complexities in an easier way.

For now, we'll just create a bare OS (if we even can call it that way) that
includes:
* kernel loading (boot),
* basic video management.

We'll use [ELF](http://wiki.osdev.org/ELF) as the kernel image format, and
[ld](http://wiki.osdev.org/LD) linker from the cross-compiler just built (see
"Pre-requisites" section above) to produce an ELF-formatted kernel image, and
[GRUB](https://wiki.osdev.org/GRUB)
[bootloader](https://wiki.osdev.org/Bootloader) for booting the kernel.

#### Why GRUB?

GRUB is so powerful and natively supports loading ELF files. Plus, it adheres
to the [Multiboot](http://wiki.osdev.org/Multiboot) specification.\
GRUB save us from all the pain of switching from Real Mode to Protected Mode,
as it handles all the unpleasant details.


## From the beginning...

When the computer is turned on (see
[Initialization](http://wiki.osdev.org/System_Initialization_(x86))),
the CPU starts in the so called [Real Mode](http://wiki.osdev.org/Real_Mode)
for compatibility reasons. In order to get all the power from an x86 CPU, we
need to enable the so called
[Protected Mode](http://wiki.osdev.org/Protected_Mode).\
We'll instruct the CPU to load GRUB code once system initialization has been
concluded.
Just before jumping to the kernel, GRUB leaves the CPU in Protected Mode with a
full 4 GiB addressing space (32 bits), and [Paging] and [Interrupts] disabled
(more on this later).

#### Why don't we just load our kernel instead of GRUB?
d

## NO STANDARD LIBRARY

In the kernel ... .. There's no standard library..!


## MULTIBOOT SPEC / GRUB

Aside,
EBX contains a pointer to the Multiboot information structure
EAX contains the value 0x2BADB002
Note that GRUB configures a stack but we can't trust its location, so we need to define ours.


## How does GRUB load the ELF-formatted kernel?

In order for GRUB to get the kernel loaded, it needs to know where in RAM
memory we want the kernel to be loaded, which is the entry point of the kernel,
and so on. It gets this information from the ELF headers contained in the
image. And the image
is created by the ld linker. (see linker.ld file)

## How is the ELF kernel image generated?
When "make" (or "make compile") is run from the project's top level directory's Makefile file, all source code is compiled (using the [GCC cross-compiler](http://wiki.osdev.org/GCC_Cross-Compiler) for C and [nasm](http://wiki.osdev.org/NASM) for ASM) into [relocatable ELF object files](http://wiki.osdev.org/Object_Files) that are linked together using ld (really using GCC as a linker) into a conclusive statically linked executable ELF file.

## How does the kernel start running?
First thing first... our kernel image is ELF formatted and its inner structure is given by the ld linker directives and commands declared in the linker.ld file (see [Linker Scripts](http://wiki.osdev.org/Linker_Scripts)).
When GRUB (or any Multiboot-compliant bootloader, for that matter) loads our kernel image, it needs to check whether the kernel is Multiboot-compliant looking for certain values to be stored at the beginning of the kernel image; that's why we have a "multiboot_header" section (see [multiboot_entry_point.asm](/kernel/multiboot_entry_point.asm)) at the very first position in the linker.ld file.
Once GRUB has checked the image, it transfers the control to the kernel executing the code at the "_start" symbol.  


TODO vEr esto: Since we haven't set up virtual memory yet, all virtual addresses are identical to the physical ones.


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



References:

* https://wiki.osdev.org/Bare_Bones
* http://wiki.osdev.org/GCC_Cross-Compiler
* http://wiki.osdev.org/Why_do_I_need_a_Cross_Compiler%3F
* http://wiki.osdev.org/LD
* https://wiki.osdev.org/System_V_ABI
* https://wiki.osdev.org/Calling_Conventions
* http://wiki.osdev.org/System_Initialization_(x86)
* http://wiki.osdev.org/Memory_Map_(x86)
* https://wiki.osdev.org/Interrupts
* https://wiki.osdev.org/Non_Maskable_Interrupt
* http://wiki.osdev.org/Real_Mode
* http://wiki.osdev.org/Protected_Mode
* https://wiki.osdev.org/Global_Descriptor_Table
* https://wiki.osdev.org/Paging
* http://wiki.osdev.org/Printing_To_Screen
* http://wiki.osdev.org/Multiboot
* http://www.jamesmolloy.co.uk/tutorial_html
* http://www.osdever.net/bkerndev/index.php
* http://www.jofre.de/?download=WritingAnOs.pdf
* https://www.gnu.org/software/grub/manual/multiboot/multiboot.html
* http://os.phil-opp.com/multiboot-kernel.html
