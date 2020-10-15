# brunix
##### =>> Small Unix-like 32-bits x86 OS for fun and learning <<=

### Stage 0: Multiboot-compliant executable

If you didn't yet, please read the main [README.md](https://github.com/bruno-masci/brunix/blob/master/README.md) 
document first, and specially the [Prerequisites](https://github.com/bruno-masci/brunix/tree/master#pre-requisites) section.

# Goals

For now, we'll just create a bootable ELF executable and check it is a valid Multiboot-compliant executable.\
In order to achieve that, we need to add the Multiboot header at the beginning of the executable and set the desired entry 
point where the control will be transferred to by the bootloader.

# Kernel building 

* please complement this section by looking at the "CMakeLists.txt" file,
* the *$* symbol indicates the shell prompt


### Compiler and linker setup
Our cross-compiler is set as follows:

    CROSS_COMPILER_BIN_PATH:  /home/osdev/opt/cross/bin
    CMAKE_C_COMPILER:         ${CROSS_COMPILER_BIN_PATH}/i686-elf-gcc
    CMAKE_ASM_COMPILER:       ${CMAKE_C_COMPILER}

When we fire the image generation process, *GCC* compiles all C and ASM source code into [relocatable ELF object files](http://wiki.osdev.org/Object_Files) that
are linked together, using *LD*, into a statically-linked ELF executable file:

    CMAKE_LINKER:             ${CROSS_COMPILER_BIN_PATH}/i686-elf-ld
    CMAKE_EXE_LINKER_FLAGS:   ${LDFLAGS} -Wl,-Map,System.map
    CMAKE_C_LINK_EXECUTABLE:  ${CMAKE_C_COMPILER} <CMAKE_C_LINK_FLAGS> <LINK_FLAGS> -o <TARGET> <OBJECTS> -lgcc

The "-Map,System.map" option creates a file called "System.map" containing all the symbols from the object files.\
Regarding the "-lgcc" library inclusion, see [Libgcc](https://wiki.osdev.org/Libgcc).


### Image validation

We can check the expected 32-bit executable was created:

    build$ file brunix.elf 
[output:]\
*brunix.elf: __ELF 32-bit LSB executable__, __Intel 80386__, version 1 (SYSV), __statically linked__, not stripped*


# x86 architecture overview

### Address spaces

On an x86 PC we have [two kind of addresses](https://www.geeksforgeeks.org/logical-and-physical-address-in-operating-system/):
*virtual/logical* and *physical* (assume *virtual* and *logical* is the same here).\
Virtual addresses (don't confuse it with [virtual memory](https://en.wikipedia.org/wiki/Virtual_memory)) are the ones the
CPU understand and programs use, while physical addresses are the ones that can be accessed by the hardware, like RAM 
memory chips, memory-mapped devices, BIOS routines, and so on.

On the one hand, an x86 machine has a 32-bit CPU and so the CPU can address up to 4 GiB of __virtual__ address. On the 
other hand, there is a physical address space that could be larger than 4 GiB
(see [PAE](https://wiki.osdev.org/Physical_Address_Extension)). I point this out to emphasize both address spaces are 
completely different.

__The CPU only knows about virtual addresses__, so no program (even the kernel) can directly access a physical address.\
x86 provides some mechanisms that act as the bridge between those address spaces (more on this later). Those mechanisms
let programs to flexibly map the virtual addresses emitted by the CPU to physical addresses.\
What we can do, for example, is to map a __virtual__ address (or region) *A* to the __physical__ address *A*, effectively
hiding the translation mechanisms and creating the illusion of being directly accessing the __physical__ address *A* (note
that we said "__a__ virtual address" and "__the__ physical address")

### Physical memory layout

The [x86 memory model](https://wiki.osdev.org/Memory_Map_(x86)) reserves some physical memory regions for special uses
(physical memory is *not* the same as RAM memory).\
The region that extends between 0xC0000000 (3 GiB) and 0xFFFFFFFF (4 GiB) contains some memory-mapped devices.\
The region that extends through the first MiB does contain the BIOS routines, the memory-mapped video display, etc. Also
in this region there are some scattered free (RAM) memory slots we choose to ignore for simplicity. 

### Linker's VIRTUAL vs LOAD address

(Although this is not particular to x86, let's keep this section here)

The linker differentiates between the address a given section runs at (the virtual address) and the address that section
is loaded -or programmed, or flashed- at (the load address).\
In our particular case, the load address would be the RAM memory address where a section is loaded at:
* by the bootloader, in the case of the kernel,
* by the kernel, in the case of a regular program.

Let's assume both addresses (numerically) match for our first model. In later stages we will benefit from this feature 
while switching to a more flexible memory layout for executing regular (user mode) programs.

# Kernel layout

Based on the previous discussion let's place the kernel at 1 MiB, but... 1 MiB of what? For now, let both virtual and load
addresses to be 1 MiB.\
In this case that means that:
1. the kernel instructions are going to start __from__ *virtual* address 1 MiB (*KERN_LINK* in "linker.ld.pp" file),
2. the bootloader will load the kernel's image __at__ 1 MiB of physical (RAM) memory.

Why 1 MiB of physical memory? That location is just a ~~convention~~ choice; it could have been any other location given
there is actual (and free) RAM memory in there.
 
![boot_args](pics/physical_layout.png)

[Figure 1 - Kernel layout in physical memory]
\
\
\
The *_start* symbol in the figure represents the entry point of the kernel's image (see next section); that is, the address
of the first kernel's instruction to be executed by the bootloader once the bootloader has loaded the kernel into physical memory.

### Digging into kernel's ELF file

    build$ /home/osdev/opt/cross/bin/i686-elf-objdump -x brunix.elf 

[partial output:]
    
    start address 0x00101000
    
    Program Header:
        LOAD off    0x00000054 vaddr 0x00100000 paddr 0x00100000 align 2**2
    
    Sections:
    Idx Name          Size      VMA       LMA       File off  Algn
      0 .boot         0000000c  00100000  00100000  00000054  2**2
                      CONTENTS, ALLOC, LOAD, READONLY, DATA
      1 .text         00000005  00101000  00101000  00001054  2**0
                      CONTENTS, ALLOC, LOAD, READONLY, CODE
    SYMBOL TABLE:
    00100000 l    d  .boot	00000000 .boot
    00101000 l    d  .text	00000000 .text
    00000000 l    df *ABS*	00000000 CMakeFiles/brunix.elf.dir/kernel/multiboot_entry_point.S.o
    00101002 l       .text	00000000 .hang
    00101005 l       .text	00000000 .end
    00101000 g       .text	00000000 _start

ZZZ

    /home/osdev/opt/cross/bin$ i686-elf-objdump -D brunix.elf 
[partial output:]

    Disassembly of section .boot:
    
    00100000 <.boot>:
      100000:	02 b0 ad 1b 00 00    	add    0x1bad(%eax),%dh
      100006:	00 00                	add    %al,(%eax)
      100008:	fe 4f 52             	decb   0x52(%edi)
      10000b:	e4                   	.byte 0xe4
    
    Disassembly of section .text:
    
    00101000 <_start>:
      101000:	eb 00                	jmp    101002 <.hang>
    
    00101002 <.hang>:
      101002:	f4                   	hlt    
      101003:	eb fd                	jmp    101002 <.hang>

SSSSSS

    /home/osdev/opt/cross/bin$ i686-elf-readelf -a brunix.elf 
[partial output:]
   
    Section Headers:
      [Nr] Name              Type            Addr     Off    Size   ES Flg Lk Inf Al
      [ 0]                   NULL            00000000 000000 000000 00      0   0  0
      [ 1] .boot             PROGBITS        00100000 000054 00000c 00   A  0   0  4
      [ 2] .text             PROGBITS        00101000 001054 000005 00  AX  0   0  1
    Key to Flags:
      A (alloc), X (execute)

    Program Headers:
      Type           Offset   VirtAddr   PhysAddr   FileSiz MemSiz  Flg Align
      LOAD           0x000054 0x00100000 0x00100000 0x01005 0x01005 R E 0x4
    
     Section to Segment mapping:
      Segment Sections...
       00     .boot .text 


# FAQ

#### Why ELF?

ELF is a very flexible, well supported and documented file format, that distinguish between TEXT, DATA and BSS sections.
Besides, ELF supports separate load and execution addresses, vital for a
[Higher Half Kernel](https://wiki.osdev.org/Higher_Half_Kernel) (more on this later).

#### Why CMake?

On the one hand, CMake has a great expressive power and allows for easy setup and management of multi-directory projects.
Plus, it integrates very well with Jetbrains' CLion, the IDE I use for developing.
On the other hand, it offers different output artifact formats, Makefile included (the one I use). 



# References

* https://en.wikipedia.org/wiki/Mebibyte
* https://wiki.osdev.org/Bare_Bones
* https://os.phil-opp.com/multiboot-kernel/
* http://wiki.osdev.org/Why_do_I_need_a_Cross_Compiler%3F
* http://wiki.osdev.org/GCC_Cross-Compiler
* https://wiki.osdev.org/Libgcc
* http://wiki.osdev.org/LD
* https://ravikiranb.com/articles/linker-script/
* http://wiki.osdev.org/Memory_Map_(x86)
* https://en.wikipedia.org/wiki/Virtual_memory
* https://www.geeksforgeeks.org/logical-and-physical-address-in-operating-system/
* https://wiki.osdev.org/Interrupts
* https://wiki.osdev.org/Non_Maskable_Interrupt
* http://wiki.osdev.org/Multiboot
* https://www.gnu.org/software/grub/manual/multiboot/multiboot.html
* http://os.phil-opp.com/multiboot-kernel.html
* https://css.csail.mit.edu/6.858/2014/readings/i386.pdf
* https://pdos.csail.mit.edu/6.828/2014/readings/pcasm-book.pdf
* http://wiki.osdev.org/ELF
* https://pdos.csail.mit.edu/6.828/2014/readings/elf.pdf
* http://derekmolloy.ie/hello-world-introductions-to-cmake/
* https://bytefreaks.net/programming-2/make-building-with-cmake-verbose