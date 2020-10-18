# brunix
##### =>> Small Unix-like 32-bits x86 OS for fun and learning <<=

### Stage 1: Emulator and VGA

If you didn't yet, please start from the main [README.md](https://github.com/bruno-masci/brunix/blob/master/README.md) 
document first.

# Goals

In this stage we are going to create a bare OS (if we even can call it that way) that includes:
* kernel loading (boot),
* basic video management.

## File structure (project root)

 * |── ...
 * |── include/
   * |── ...
   * |── *brunix/* ----------> architecture-independent header files.
 * |── *libkern/* ---------------> custom C library for the kernel.
 * |── qemu.cfg.tmpl -----> config file for QEMU (template).
 * |── bochs.cfg.tmpl -----> config file for Bochs (template).
####[only added files showed]

## File structure (build/ directory)

 * |── build/ 
   * |── ...
   * |── brunix.iso ------------> bootable ISO image for the kernel.
   * |── qemu.cfg -------------> resolved config file for QEMU.
   * |── bochs.cfg ------------> resolved config file for Bochs.
####[only added files showed]


- -
We'll use [ELF](http://wiki.osdev.org/ELF) as the kernel image format, [ld](http://wiki.osdev.org/LD) linker from the
cross-compiler just built (see "[Pre-requisites](#Pre-requisites)" section above) to produce an ELF-formatted kernel image,
and [GRUB](https://wiki.osdev.org/GRUB) [bootloader](https://wiki.osdev.org/Bootloader) for booting the kernel.
- -

## Build target

* the *$* symbol indicates the shell prompt

### Supported build commands

	make qemu
runs the kernel's image on QEMU,
 
	make bochs
runs the kernel's image on Bochs,

# Kernel building 

* please complement this section by looking at the "CMakeLists.txt" file,

#### *qemu* target

Runs QEMU emulator simulating that the OS image (brunix.iso, an ISO file) is inserted in the CD-ROM drive of a machine 
with 512 MiB of RAM memory:

	qemu-system-i386 -cdrom brunix.iso -m 512M

#### *bochs* target

Like *qemu*, but reading the configuration from the "bochsrc.txt" file.

	megs: 512
	ata0-slave:  type=cdrom, path="brunix.iso", status=inserted
    boot: cdrom


## Running the kernel

We can run it with:

	make qemu
	
or:

	make bochs

See #FAQs!

At boot time we can edit the boot parameters, like this:

![boot_args](pics/boot_args.png)

After the system has booted up, something like this will appear:

![booting](pics/booting.png)


# Kernel loading

Affected files:
* linker.ld.pp
* kernel/multiboot_entry_point.S
* kernel/multiboot.c
* kernel/main.c

Let's see what happen when we try to boot from  a CD containing a bootable ISO file:

##### Ancient ages

When the computer is turned on (see [Initialization](http://wiki.osdev.org/System_Initialization_(x86))), the CPU starts
in the so called Real Mode for compatibility reasons. In order to get all the power from an x86 CPU, we need to enable
the so called Protected Mode.\
So far, we have an ISO file containing both GRUB code (for booting) and the ELF kernel image.\
Once system initialization has been concluded, the ISO file is read from the CD-ROM drive and GRUB code gets executed.\
GRUB knows where the kernel is in the storage (see "iso/boot/grub/grub.cfg" file) and where to load it in physical RAM
memory for execution - 1 MiB, in this case - (see "linker.ld" file).
Just before jumping to the kernel, GRUB switches the CPU to Protected Mode with a full 4 GiB addressing space (32 bits),
and [Paging](https://wiki.osdev.org/Paging) and [Interrupts](https://wiki.osdev.org/Interrupts) disabled (more on this later).


### How is the brunix.iso file created?

When we run <i>"make"</i> or <i>"make compile"</i> (see "Makefile" file) from the project's top level directory, once
"brunix.elf" file gets created and put into "iso" directory, an ISO image is constructed with:

	grub-mkrescue -d misc/grub/i386-pc -o os.iso iso/

IMPORTANT: If you are working on an amd64 platform (as in my case) you need to provide the "-d" option to the command above.




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


## Some considerations about GCC/GAS

* Default *includes* like `#include <stddef.h>` and `#include <stdint.h>` are __not__ from the [standard C library](https://wiki.osdev.org/C_Library) (we don't have one in kernel space) but from the compiler itself.
* In the context of including header files, we use the `__ASSEMBLER__` macro (like in `#ifndef __ASSEMBLER__`) to include C Preprocessor's structures on GNU assembler source files while ignoring any C code (like structs or unions) that would end up included by default.
* GCC's `__attribute__((packed))` is used to avoid paddings by the compiler: we need some structures' fields to be exactly on a particular location, at bit level.
* Note we don't use GCC's `__attribute__ ((noreturn))` for functions that don't return, like *panic()*: this is to avoid optimizations by the compiler that would play against debugging. We only mark those functions as NORET_FUNC (see [defs.h](/include/brunix/defs.h)) for informational purposes only.



# FAQ

#### Why GRUB?

GRUB, which adheres to the [Multiboot](http://wiki.osdev.org/Multiboot) specification, is very powerful and natively
supports loading ELF files.\
GRUB save us from all the pain of switching from [Real Mode](http://wiki.osdev.org/Real_Mode) to
[Protected Mode](http://wiki.osdev.org/Protected_Mode), as it handles all the unpleasant details. Also, GRUB helps us
avoid having to call [BIOS](https://wiki.osdev.org/BIOS) services.

#### Why both QEMU and Bochs?

QEMU is faster than Bochs and also integrates with GDB (more on this later), so it have its place as the regular emulator.
But, as the project evolves, it is a good idea to run it on Bochs from time to time given Bochs is way more accurate than 
QEMU on x86.\

TODO VER ESTO!!!!\
In other regards, Bochs don't support command line configuration and QEMU accept both command line configuration and
configuration file. We stick to the command line configuration for QEMU for debugging reasons: 


# References (cont.)

* https://wiki.osdev.org/Bare_Bones
* https://wiki.osdev.org/System_V_ABI (en duda)
* https://wiki.osdev.org/Calling_Conventions (en duda)
* http://wiki.osdev.org/System_Initialization_(x86)
* https://learning.oreilly.com/library/view/programming-embedded-systems/0596009836/ch04.html
* https://0xax.gitbooks.io/linux-insides/content/
* http://wiki.osdev.org/Memory_Map_(x86)
* https://wiki.osdev.org/Interrupts
* https://wiki.osdev.org/Non_Maskable_Interrupt
* https://pdos.csail.mit.edu/6.828/2014/readings/pcasm-book.pdf
* https://wiki.osdev.org/Inline_Assembly
* https://en.wikibooks.org/wiki/The_Linux_Kernel/System
* https://gcc.gnu.org/onlinedocs/gcc-3.3/gcc/C-Extensions.html
* http://www.jamesmolloy.co.uk/tutorial_html
* http://www.osdever.net/bkerndev/index.php
* http://wiki.osdev.org/Printing_To_Screen
* http://www.brokenthorn.com/Resources/OSDevVga.html
* http://wiki.osdev.org/Real_Mode
* http://wiki.osdev.org/Protected_Mode

* https://wiki.osdev.org/Serial_Ports ??|
* //https://wiki.osdev.org/Global_Descriptor_Table ??