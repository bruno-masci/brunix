# brunix (stage 0)
##### *** Small Unix-like 32-bits x86 OS for fun and learning ***

## Stage 0 - Project layout and Multiboot-compliant executable


If you didn't yet, please read the main [README.md](https://github.com/bruno-masci/brunix/blob/master/README.md) document first.

### Goals

For now, we'll just create a bootable ELF executable and check it is a valid Multiboot-compliant executable.\
In order to achieve that, we need to add the Multiboot header at the beginning of the executable and set a desired entry point where the control is transferred to by the bootloader.


## File structure

 * |── CMakeLists.txt ------> *CMake*'s build specification.
 * |── include/
   * |── *arch/x86/* -------> x86 architecture-dependent header files.
 * |── *kernel/* ----------------> kernel source code.
 * |── *linker.ld.pp* ----------> linker script for preprocessing.
 * |── multiboot/ -----------> Multiboot related files.
 * |── build/ -----------------> build dir for *CMake*. 


## Project building

Is is highly recommended to build the project on a directory other than the source directory. We assume *build/* as the build directory but any other will work too.\
__Any command that appears below is assumed to be run from the *build/* directory__ (the $ symbol indicates the shell prompt).

For building the kernel, we need to run (this will create a *Makefile* file):

	$ cmake ../

and then:

	$ make

we will end up with:

 * |── build/ 
   * |── Makefile -------------> *make*'s build specification.
   * |── *brunix.elf* ------------> kernel's image.
   * |── *linker.ld* --------------> kernel's preprocessed linker script.
   * |── System.map --------> kernel's symbol table. 
   * |── brunix.asm ----------> kernel's disassembled image.
   * |── brunix.iso ------------> bootable ISO image for the kernel.
   * |── iso/ --------------------> *grub-mkrescue*'s expected directory layout for creating an ISO file.


## Build targets (*make*)

	<<default>>
(i.e.: just "*make*") builds the kernel's image,

	clean
removes the generated kernel's image,

	clean-all
removes all generated files but *make*/*CMake*'s own files.


### Behind the scenes

Please complement this section by looking at the "CMakeLists.txt" file.

#### 'default' target

(repetido)
Using the preconfigured* GCC cross-compiler, it compiles all C and ASM ([GAS](http://wiki.osdev.org/GAS)) source code into [relocatable ELF object files](http://wiki.osdev.org/Object_Files) that
are linked together using ld (actually using GCC as a linker) into a conclusive statically linked ELF executable file:

    CMAKE_EXE_LINKER_FLAGS:   ${LDFLAGS} -Wl,-Map,${SYSTEM_MAP_NAME}
    CMAKE_C_LINK_EXECUTABLE:  ${CMAKE_C_COMPILER} <CMAKE_C_LINK_FLAGS> <LINK_FLAGS> -o <TARGET> <OBJECTS> -lgcc

The "-Map,System.map" option creates a file called "System.map" containing all the symbols from the ELF image.\
Regarding the "-lgcc" library inclusion, see [Libgcc](https://wiki.osdev.org/Libgcc).




- - (repetido)
We'll use [ELF](http://wiki.osdev.org/ELF) as the kernel image format, [ld](http://wiki.osdev.org/LD) linker from the
cross-compiler just built (see "[Pre-requisites](#Pre-requisites)" section above) to produce an ELF-formatted kernel image,
and [GRUB](https://wiki.osdev.org/GRUB) [bootloader](https://wiki.osdev.org/Bootloader) for booting the kernel.
- -



## Image building and validation

From the *build/* directory, we must run:

	$ make

and expect to see something like:

    [100%] Built target brunix.elf


## How do pieces play together?

### Kernel loading


Let's see what happen when we try to boot from  a CD containing a bootable ISO file:


### How is the brunix.iso file created? (repetido)

When we run <i>"make"</i> or <i>"make compile"</i> (see "Makefile" file) from the project's top level directory, once
"brunix.elf" file gets created and put into "iso" directory, an ISO image is constructed with:

	grub-mkrescue -d multiboot/misc/grub/i386-pc -o os.iso iso/

IMPORTANT: If you are working on an amd64 platform (as in my case) you need to provide the "-d" option to the command above.




## How does GRUB load the ELF-formatted kernel? (repetido)

In order for GRUB to get the kernel loaded, it needs to know where in RAM
memory we want the kernel to be loaded, which is the entry point of the kernel,
and so on. It gets this information from the ELF headers contained in the
image. And the image
is created by the ld linker. (see linker.ld file)

## How does the kernel start running? (repetido)
First thing first... our kernel image is ELF formatted and its inner structure is given by the ld linker directives and commands declared in the linker.ld file (see [Linker Scripts](http://wiki.osdev.org/Linker_Scripts)).
When GRUB (or any Multiboot-compliant bootloader, for that matter) loads our kernel image, it needs to check whether the kernel is Multiboot-compliant looking for certain values to be stored at the beginning of the kernel image; that's why we have a "multiboot_header" section (see [multiboot_entry_point.S](/kernel/multiboot_entry_point.S)) at the very first position in the linker.ld file.
Once GRUB has checked the image, it transfers the control to the kernel executing the code at the "_start" symbol.


### FAQ

#### Why ELF? (repetido)

ELF is a very flexible, well supported and documented file format, that distinguish between TEXT, DATA and BSS sections.
Besides, ELF supports separate load and execution addresses, vital for a
[Higher Half Kernel](https://wiki.osdev.org/Higher_Half_Kernel) (more on this later).

#### Why GRUB? (repetido)

GRUB, which adheres to the [Multiboot](http://wiki.osdev.org/Multiboot) specification, is very powerful and natively
supports loading ELF files.\
GRUB save us from all the pain of switching from [Real Mode](http://wiki.osdev.org/Real_Mode) to
[Protected Mode](http://wiki.osdev.org/Protected_Mode), as it handles all the unpleasant details. Also, GRUB helps us
avoid having to call [BIOS](https://wiki.osdev.org/BIOS) services.





##References:

* https://css.csail.mit.edu/6.858/2014/readings/i386.pdf
* https://wiki.osdev.org/Bare_Bones
* http://wiki.osdev.org/GCC_Cross-Compiler
* http://wiki.osdev.org/Why_do_I_need_a_Cross_Compiler%3F
* http://wiki.osdev.org/LD
* http://wiki.osdev.org/Memory_Map_(x86)
* //https://wiki.osdev.org/Interrupts
* //https://wiki.osdev.org/Non_Maskable_Interrupt
* http://wiki.osdev.org/Multiboot
* https://www.gnu.org/software/grub/manual/multiboot/multiboot.html
* http://os.phil-opp.com/multiboot-kernel.html
* https://en.wikibooks.org/wiki/The_Linux_Kernel/System
* https://wiki.osdev.org/Libgcc

https://pdos.csail.mit.edu/6.828/2014/readings/elf.pdf
https://pdos.csail.mit.edu/6.828/2014/readings/pcasm-book.pdf

https://gcc.gnu.org/onlinedocs/gcc-3.3/gcc/C-Extensions.html