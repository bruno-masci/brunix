# brunix
Small Unix-like x86 OS for fun and learning (stage 0)

Pre-requisites:
mtools
xorriso
grub
nasm
A proper cross compiler

here [see http://wiki.osdev.org/GCC_Cross-Compiler]
[a link](https://github.com/user/repo/blob/branch/other_file.md)

In this very first stage we are going to outline and define the skeleton of the system. It will define
a primitive OS for running on x86 (386+) architecture.. and includes:
* kernel loading (boot)
* basic video management

When the computer [boots](http://wiki.osdev.org/System_Initialization_(x86)),
the CPU starts in the so called [Real Mode](http://wiki.osdev.org/Real_Mode) for compatibility reasons. In order
to get all the power from an 386+ CPU, we need to enable the so called [Protected Mode](http://wiki.osdev.org/Protected_Mode).

As the bootloader we will use GRUB (GRUB 2, in particular). GRUB save us all the pain of switching from Real Mode to Protected Mode, as it handles
all the unpleasant details and leaves the CPU in Protected Mode with a full 4 GiB addressing space (32 bits)
and paging and interrupts disabled.
We will take advantage of GRUB's native support for loading ELF files, and so we are going to use the flexible ELF format for our kernel.
For creating the kernel we are going to use the linker (ld) from the cross-compiler just built as the first step
to produce an ELF formatted kernel image.



Aside,
EBX contains a pointer to the Multiboot information structure
EAX contains the value 0x2BADB002
Note that GRUB configures a stack but we can't trust its location, so we need to define ours.


NOTE: The information here is complemented with that contained in the source code.
Now.. What happens when we instruct GRUB to load our ELF kernel image?
In order for GRUB to get the kernel loaded, it needs to know where in RAM memory we want the kernel to be loaded, which
is the entry point of the kernel, and so on. It gets this information from the ELF headers contained in the image. And the image
is created by the ld linker. (see linker.ld file)
to parse the ELF headers so it retrieves uses the information stored it

References:

http://www.jamesmolloy.co.uk/tutorial_html

http://wiki.osdev.org

http://os.phil-opp.com/multiboot-kernel.html

http://www.osdever.net/bkerndev/index.php

http://www.jofre.de/?download=WritingAnOs.pdf

