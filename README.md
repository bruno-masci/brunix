# brunix (stage 0)
##### *** Small Unix-like 32-bits x86 OS for fun and learning ***

### =>>> Multiboot-compliant executable <<<=


If you didn't yet, please read the main [README.md](https://github.com/bruno-masci/brunix/blob/master/README.md) 
document first, and specially the [Pre-requisites](https://github.com/bruno-masci/brunix/tree/master#-pre-requisites).

## # Goals

For now, we'll just create a bootable ELF executable and check it is a valid Multiboot-compliant executable.\
In order to achieve that, we need to add the Multiboot header at the beginning of the executable and set the desired entry point where the control is transferred to by the bootloader.

## # Building internals 

(Please complement this section by looking at the "CMakeLists.txt" file)

### Kernel image generation

When we fire the image generation process, *GCC* compiles all C and ASM source code into [relocatable ELF object files](http://wiki.osdev.org/Object_Files) that
are linked together using *LD* into a statically-linked ELF executable file:

    CMAKE_EXE_LINKER_FLAGS:   ${LDFLAGS} -Wl,-Map,System.map
    CMAKE_C_LINK_EXECUTABLE:  ${CMAKE_C_COMPILER} <CMAKE_C_LINK_FLAGS> <LINK_FLAGS> -o <TARGET> <OBJECTS> -lgcc

The "-Map,System.map" option creates a file called "System.map" containing all the symbols from the object files.\
Regarding the "-lgcc" library inclusion, see [Libgcc](https://wiki.osdev.org/Libgcc).

## Image building and validation

__Any command that appears below in this document is assumed to be run from the *build/* directory__ (the *$* symbol indicates the shell prompt).

From the *build/* directory, we must run:

	$ make

and expect to see something like:

    [100%] Built target brunix.elf

We can check the expected 32-bit executable was created:

    $ file brunix.elf 
[output:]\
*brunix.elf: __ELF 32-bit LSB executable__, __Intel 80386__, version 1 (SYSV), __statically linked__, not stripped*

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


## BLE

    $ /home/osdev/opt/cross/bin/i686-elf-objdump -x brunix.elf 
######(partial output...)
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

    $ /home/osdev/opt/cross/bin/i686-elf-objdump -D brunix.elf 
######(partial output...)
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

    $ /home/osdev/opt/cross/bin/i686-elf-readelf -a brunix.elf 
######(partial output...)    
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

ssssaaa
    

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





## References

* https://en.wikipedia.org/wiki/Mebibyte
* https://wiki.osdev.org/Bare_Bones
* http://wiki.osdev.org/Why_do_I_need_a_Cross_Compiler%3F
* http://wiki.osdev.org/GCC_Cross-Compiler
* https://wiki.osdev.org/Libgcc
* http://wiki.osdev.org/LD
* http://wiki.osdev.org/Memory_Map_(x86)
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