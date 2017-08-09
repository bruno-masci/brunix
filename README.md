# brunix
Small Unix-like x86 OS for fun and learning (stage 0)

Note: We assume 386+ when we refer to x86 architecture.

Pre-requisites:
mtools
xorriso
grub
nasm
A proper cross compiler

here [see http://wiki.osdev.org/GCC_Cross-Compiler]

In this very first stage we are going to outline and depict the project structure. The idea is to do incremental developments (stage0, stage1, ...) to tackle all the complexities in an easier way.

For now, we'll just create a bare OS (if we even can call it that way) that includes:
* kernel loading (boot)
* basic console management

We'll use [ELF](http://wiki.osdev.org/ELF) as the kernel format, because it's a very well supported, portable and flexible format.

When the computer [boots](http://wiki.osdev.org/System_Initialization_(x86)),
the CPU starts in the so called [Real Mode](http://wiki.osdev.org/Real_Mode) for compatibility reasons. In order
to get all the power from an x86 CPU, we need to enable the so called [Protected Mode](http://wiki.osdev.org/Protected_Mode).

As the bootloader we will use GRUB (GRUB 2, in particular). GRUB save us all the pain of switching from Real Mode to Protected Mode, as it handles
all the unpleasant details and leaves the CPU in Protected Mode with a full 4 GiB addressing space (32 bits),
and paging and interrupts disabled.
We will take advantage of GRUB's native support for loading ELF files, and so we are going to use the flexible ELF format for our kernel.
For creating the kernel we are going to use the linker (ld) from the cross-compiler just built as the first step
to produce an ELF formatted kernel image.

There's no standard library..!

Aside,
EBX contains a pointer to the Multiboot information structure
EAX contains the value 0x2BADB002
Note that GRUB configures a stack but we can't trust its location, so we need to define ours.



* kernel
** main.c: The C kernel's entry point.
** vga.c: Basic video routines.


.
 * |── brunix.elf
 * |── brunix-nosym.elf
 * |── brunix.sym
 * |── brunix-nosym.elf
 * |── include
   * |── asm-x86: x86 architecture-dependent header files.
   * |── brunix: architecture-independent header files.
 * |── kernel: kernel source code.
 * |── libkern: custom libc for the kernel.
 * |── linker.ld
 * |── Makefile
 * |── Makefile.inc
 * |── linker.ld
 * |── os.iso
 * |── System.map
 
. 
├── iso
│   └── boot
│       ├── brunix.elf
│       └── grub
│           └── grub.cfg
├── kernel
├── 
├── linker.ld
├── Makefile
├── Makefile.inc
├── os.iso
├── README.md
├── System.map
└── test
    ├── atoi.c
    └── atoi-test


References:

http://www.jamesmolloy.co.uk/tutorial_html
http://wiki.osdev.org
http://os.phil-opp.com/multiboot-kernel.html
http://www.osdever.net/bkerndev/index.php
http://www.jofre.de/?download=WritingAnOs.pdf

