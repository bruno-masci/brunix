# brunix (in progress)
##### =>> Small Unix-like 32-bits x86 OS for fun and learning <<=

Before starting, please note that:
* the information here is complemented with that contained in the source code,
* development is split in Git branches (stages), where each branch builds on top of the previous one,
* with x86 we mean 386+ architecture.


# Pre-requisites
* A <b>proper</b> [cross-compiler](http://wiki.osdev.org/GCC_Cross-Compiler)
* cmake
* make
* bash
* grub
* xorriso library (for creating the ISO image)


# Overview

This is a very simple x86 operating system I have been working on for years. 
Through all these years I have been researching, learning, testing about different aspects of operating systems and also
trying to understand how the building blocks play together.

My interest on OSs traces back to when I was young. I can say there are tons of information on OSs scattered
all around the Internet. In my humble opinion, the problem with that is we can't trust it or confuses us 
rather than it enlightens: there are countless 
projects/solutions half done, based on misconceptions, there are hidden assumptions, there are erroneous 
usage of data types, incorrect setup of low-level data structures, and so on. 
My hope is that this guide helps others to follow a logical path towards the building of a simple operating system,
learning about the important concepts and details in the process.

Once I have finished the development of the OS, this page will contain general information about building and using it.
Apart from that, you can go step by step and grasp some concepts as needed (see below).


# Using this guide

I have split the development in different Git branches, starting from the branch *stage0* to some branch *stageN*, following
a logical path for that organization.
Each branch extends the previous one, adding functionalities and explanations. \
You can compare two adjacent branches to see exactly what changed. The idea here is that you complement both the source code and the README.md file while comparing. You will find a lot more of information and details in the other branches.

* [stage0-multiboot](https://github.com/bruno-masci/brunix/tree/stage0-multiboot)
* [stage1-emulator_and_vga](https://github.com/bruno-masci/brunix/tree/stage0-emulator_and_vga)
* [stage2-kernel_debugging]() (in progress)
* ...


# Toolchain specs
 * Kernel image format: [ELF](http://wiki.osdev.org/ELF),
 * C compiler: [GCC](http://wiki.osdev.org/GCC)
 * Assembler: GCC/[GAS](http://wiki.osdev.org/GAS),
 * Linker: [LD](http://wiki.osdev.org/LD),
 * [Bootloader](https://wiki.osdev.org/Bootloader): [GRUB](https://wiki.osdev.org/GRUB).


# File structure

 * |── CMakeLists.txt ------> *CMake*'s build specification.
 * |── include/
   * |── *arch/x86/* -------> x86 architecture-dependent header files.
 * |── *kernel/* ----------------> kernel source code.
 * |── *linker.ld.pp* ----------> linker script for preprocessing.
 * |── multiboot/ -----------> Multiboot related files.
 * |── build/ -----------------> build dir for *CMake*.


# Project building

### Build targets (*make*)

	<default>
(i.e.: just "*make*") builds the kernel's image,

	clean
removes the generated kernel's image,

	clean-all
removes all generated files but *make*/*CMake*'s own files.

### Building the kernel
Is is highly recommended to build the project on a directory other than the source directory. We assume *build/* as the build directory but any other will work too.\
(the *$* symbol indicates the shell prompt)

To build the kernel, we need to:
1. build the CMake project => this will create a *Makefile* file,
2. build the Make project from the generated *Makefile* file


	build$ cmake ../

	build$ make

[output:]

    [100%] Built target brunix.elf
    
If everything went well, we should end up with this layout:

 * |── build/ 
   * |── Makefile -------------> *make*'s build specification.
   * |── *brunix.elf* ------------> kernel's image.
   * |── *linker.ld* --------------> kernel's preprocessed linker script.
   * |── System.map --------> kernel's symbol table. 
   * |── brunix.asm ----------> kernel's disassembled image.
   * |── brunix.iso ------------> bootable ISO image for the kernel.
   * |── iso/ --------------------> *grub-mkrescue*'s expected directory layout for creating an ISO file.



# References

* https://wiki.osdev.org
* http://www.cse.iitd.ernet.in/os-lectures
* http://www.jamesmolloy.co.uk/tutorial_html
* http://www.osdever.net/bkerndev/index.php
* (a lot more on the different stages)