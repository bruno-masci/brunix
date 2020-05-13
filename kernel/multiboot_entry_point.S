/* The kernel starts executing in this file. This file is linked with
 * the kernel C code, so it can refer to kernel symbols such as kmain().
 * (see https://www.gnu.org/software/grub/manual/multiboot/multiboot.html)
 */


#include <arch/x86/multiboot.h>
#include <arch/x86/mmu.h>
#include <arch/x86/memlayout.h>
#include <arch/x86/asm.h>


///////////////////////////////////////////////////////////////////////////////
// ********************* SECTION: multiboot_header ************************* //
///////////////////////////////////////////////////////////////////////////////

/* Here we declare the Multiboot header that marks the program as a kernel. The
 * bootloader will search for this header in the first 8 KiB of the kernel file.
 * This header must be longword (32-bit) aligned (".align 4"), and it is in
 * its own section so we can put it at the beginning of the kernel image file
 * (see "linker.ld.pp" file).
 * Note that we need to force the flag "a" (allocatable) because this is not a
 * default text section (like .text or .rodata).
 */
.section .multiboot_header, "a"
.align 4
    .long MBOOT_HEADER_MAGIC
    .long MBOOT_HEADER_FLAGS
    .long MBOOT_HEADER_CHECKSUM



///////////////////////////////////////////////////////////////////////////////
// **************************** SECTION: bss ******************************* //
///////////////////////////////////////////////////////////////////////////////

.section .bss #@nobits
.align 16

/* The Multiboot standard doesn't define a value for the stack pointer register
 * (esp) so it is up to the kernel to provide a stack.
 * Here we allocate room for a small temporal stack by creating a symbol at the
 * bottom of it, then allocating BOOTSTRAP_STACK_SIZE bytes for it, and finally
 * creating a symbol at the top of it. Please note that the stack grows
 * downwards on x86.
 * This stack is in its own section so it can be marked 'nobits', which means
 * the kernel file is smaller because it doesn't contain an uninitialized stack.
 * The stack on x86/GCC must be 16-byte aligned according to the System V ABI
 * standard (see https://wiki.osdev.org/System_V_ABI). The compiler will assume
 * the stack is properly aligned and failure to align the stack will result in
 * undefined behavior.
 */
.set BOOTSTRAP_STACK_SIZE, 16384  # Size of bootstrap stack in bytes (16 KiB).

.align 4
#stack_bottom:
#    .skip BOOTSTRAP_STACK_SIZE, 4
#stack_top:
.comm stack_top, BOOTSTRAP_STACK_SIZE
//TODO puedo usar una de estas dos variantes, pero la de arriba sin @nobits, sino arranca desde addr 0 el simbolo.


// this sections has (LA 1 MiB - VA 1 MiB) + multiboot size
.section .init, "ax"
# Entering kernel, with paging off.
.globl _start
_start:
    cli
    #movl    $88, %edx
    #movl    %edx, 0xB8000
    #movl    %edx, 0xB8001

    # Use a bootstrap GDT that makes virtual addresses map directly to physical
    # addresses so that the effective memory map doesn't change during the
    # transition.
    #pushl $gdtdesc
    #call VIRT_TO_PHYS_WO(__gdt_flush)
    lgdt    gdtdesc


    movl $stack_top, %esp


    # Save eax (contains the Multiboot magic number used later)
    movl    %eax, %ecx


    #call __load_page_directory
    #call __enable_paging

    # Turn on page size extension for 4Mbyte pages
    movl    %cr4, %eax
    orl     $MMU_CR4_PSE, %eax
    movl    %eax, %cr4

    # Set page directory
    movl    $(VIRT_TO_PHYS_WO(entrypgdir)), %eax
    movl    %eax, %cr3

    # Turn on paging
    movl    %cr0, %eax
    orl     $(MMU_CR0_PG | MMU_CR0_WP), %eax
    movl    %eax, %cr0

    # Restore eax (Multiboot magic number)
    movl    %ecx, %eax



pushl %esp
push $stack_top
pushl %eax
pushl %ebx
call kmain

# Bootstrap GDT
#.p2align 2                                # force 4 byte alignment
gdt:
    SEG_NULLASM                             # null seg
    SEG_ASM(STA_X | STA_R, 0x0, 0xffffffff)   # code seg
    SEG_ASM(STA_W, 0x0, 0xffffffff)         # data seg

gdtdesc:
    .word   (gdtdesc - gdt - 1)             # sizeof(gdt) - 1
    .long   gdt                             # address gdt





///////////////////////////////////////////////////////////////////////////////
// *************************** SECTION: text ******************************* //
///////////////////////////////////////////////////////////////////////////////

# The linker script (see "linker.ld.pp" file) specifies _start as the entry point
# to the kernel and the bootloader will jump to this position once the kernel
# has been loaded. It doesn't make sense to return from this function as the
# bootloader is gone.
.section .text

# By convention, the _start symbol specifies the ELF entry point.
# Given that GRUB needs the virtual address of the entry point and the physical and virtual address spaces does not match anymore,
# we can get rid of the original entry point and reuse the '_start' symbol name.






    /* The bootloader has loaded us into 32-bit protected mode on a x86 machine.
     * Interrupts are disabled. Paging is disabled. The processor state is as
     * defined in the Multiboot standard. The kernel has full control of the
     * CPU. The kernel can only make use of hardware features and any code it
     * provides as part of itself. There's no printf function, unless the kernel
     * provides its own <stdio.h> header and a printf implementation.
     */

	/* To set up a stack, we set the esp register to point to the top of our
	 * stack (as it grows downwards on x86 systems). This is necessarily done
	 * in assembly as languages such as C cannot function without a stack.
     */
    movl $stack_top, %esp

#FIXME
# Jump to main(), and switch to executing at
  # high addresses. The indirect call is needed because
  # the assembler produces a PC-relative instruction
  # for a direct jump.
#  pushl %eax
#  pushl %ebx
#  pushl   %ebp
#  mov    %esp,%ebp
#  sub    $8,%esp
  #mov $kmain, %edx
  #jmp *%edx

	/* Enter the high-level kernel. The ABI requires the stack is 16-byte
	 * aligned at the time of the call instruction (which afterwards pushes the
	 * return pointer of size 4 bytes). The stack was originally 16-byte aligned
	 * above and we've since pushed a multiple of 16 bytes to the stack since
	 * (pushed 0 bytes so far) and the alignment is thus preserved and the call
	 * is well defined.
	 */
	pushl %eax      # Tell the OS it was loaded by a Multiboot-compliant
	                # bootloader (see include/arch/x86/multiboot.h and
                    # kernel/main.c)
	pushl %ebx      # Save the 32-bit physical address of the Multiboot
	                # information structure provided by the boot loader.
	#call kmain      # Call kmain (defined in C), passing in the multiboot info,
	                # magic number and stack pointer (see kernel/main.c) (see
	                # https://wiki.osdev.org/Calling_Conventions).

	/* If the system has nothing more to do, put the computer into an infinite
	 * loop. To do that:
     * 1) Disable interrupts with 'cli' (see https://wiki.osdev.org/Interrupts).
     *    Please note that at this point kmain() has enabled interrupts, so we
     *    need to disable them. This applies only to maskable interrupts.
     * 2) Wait for the next interrupt to arrive with 'hlt' (halt instruction).
     *    This will stop the CPU, but as soon as a non-maskable interrupt (NMI)
     *    arrives (see https://wiki.osdev.org/Non_Maskable_Interrupt) the CPU
     *    will restart, so we need a way to keep halting all the time ('jmp')
	 */
	cli
.hang:
    hlt
    jmp .hang
.end:

    // Note we don't have an exit() function here; there is no processes nor
    // system calls. Instead we do: