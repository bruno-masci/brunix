/**
 * @author Bruno Masci
 * @brief Multiboot entry point
 * @see /linker.ld.pp
 * @see /kernel/main.c
 * @see /kernel/multiboot.c
 * @see https://wiki.osdev.org/Interrupts
 * @see https://wiki.osdev.org/Non_Maskable_Interrupt
 * @see https://wiki.osdev.org/Multiboot
 * @see http://wiki.osdev.org/System_Initialization_(x86)
 * @see https://wiki.osdev.org/System_V_ABI
 * @see https://wiki.osdev.org/Calling_Conventions
 * @see https://wiki.osdev.org/Stack
 *
 * The kernel starts executing in this file.
 */


#include <asm/multiboot.h>     // for BOOT_HEADER_MAGIC, MBOOT_HEADER_FLAGS, MBOOT_HEADER_CHECKSUM
#include <asm/memlayout.h>
#include <asm/mmu.h>
#include <asm/segment.h>


// Hand-made stab for the stack backtrace (see /kernel/kdebug.c)
.stabs	"/kernel/multiboot_entry_point.S",100,0,0,.text


///////////////////////////////////////////////////////////////////////////////
// ********************* SECTION: multiboot_header ************************* //
///////////////////////////////////////////////////////////////////////////////

/* Here we declare the Multiboot header that marks the program as a kernel. The bootloader will search for this header
 * in the first 8 KiB of the kernel file. This header must be longword (32-bit) aligned (".align 4"), and it is in its
 * own section so we can put it at the beginning of the kernel image file (see "/linker.ld.pp" file).
 * Note that we need to explicitly set the flag "a" (allocatable) because this is not a default text section
 * (like .text or .rodata).
 */
.section .multiboot_header, "a"

.align 4
    .long MBOOT_HEADER_MAGIC
    .long MBOOT_HEADER_FLAGS
    .long MBOOT_HEADER_CHECKSUM



///////////////////////////////////////////////////////////////////////////////
// **************************** SECTION: data ****************************** //
///////////////////////////////////////////////////////////////////////////////

.section .data

/* The Multiboot standard doesn't define a value for the stack pointer register (%esp) so it is up to the kernel to
 * provide a stack (see https://wiki.osdev.org/Stack).
 * Here we allocate room for a small temporal stack by creating a symbol at the bottom of it, then allocating
 * BOOTSTRAP_STACK_SIZE bytes for it, and finally creating a symbol at the top of it. Please note that the stack
 * grows *downwards* on x86.
 * The stack on x86/GCC must be 16-byte aligned according to the System V ABI standard
 * (see https://wiki.osdev.org/System_V_ABI).
 * The compiler will assume the stack is properly aligned and failure to align the stack will result in undefined
 * behavior.
 */
.set BOOTSTRAP_STACK_SIZE, 16384    # Size of bootstrap stack in bytes (16 KiB).

.align 16
bootstrap_stack_bottom:
    .space BOOTSTRAP_STACK_SIZE
bootstrap_stack_top:



///////////////////////////////////////////////////////////////////////////////
// *************************** SECTION: text ******************************* //
///////////////////////////////////////////////////////////////////////////////

.section .text

/* The linker script (see "/linker.ld.pp" file) specifies _start as the entry point to the kernel and the bootloader
 * will jump to this position once the kernel has been loaded. It doesn't make sense to return from here as the
 * bootloader is gone.
 */

# By convention, the _start symbol specifies the ELF entry point.
.globl _start
_start:

    /* The bootloader has loaded us into 32-bit Protected Mode. Entering kernel...
     * Interrupts are disabled (by the bootloader). Paging is disabled. The processor state is as defined in the
     * Multiboot standard. The kernel has full control over the CPU. The kernel can only make use of hardware features
     * and any code it provides as part of itself. There's no printf function, unless the kernel provides its own
     * <stdio.h>-like header and a printf() implementation (more on this later).
     *
     * At this point, the Multiboot-compliant bootloader should have set:
     *      %eax -> Multiboot magic number (see "/include/asm/multiboot.h" and "/kernel/main.c")
     *      %ebx -> Multiboot information structure's 32-bit *physical* address
     */

    # Clear the frame pointer register (EBP) so that once we get into debugging C code, stack backtraces will be
    # terminated properly.
	movl $0x0, %ebp			# nuke frame pointer

    movl $bootstrap_stack_top, %esp     # Set up a stack, setting %esp register to point to the top of our stack (as it
                                        # grows downwards on x86 systems). This is necessarily done in assembly as
                                        # languages such as C cannot function without a stack.


# We don't use %eax/%ax in order to keep the value stored by the bootloader on %eax; we use %esi/%si as an alternative, instead.

    # Turn on page size extension for 4Mbyte pages
    movl    %cr4, %esi
    orl     $(MMU_CR4_PSE), %esi
    movl    %esi, %cr4
    # Set page directory
    movl    $(VIRT_TO_PHYS_WO(entrypgdir)), %esi
    movl    %esi, %cr3
    # Turn on paging.
    movl    %cr0, %esi
    orl     $(MMU_CR0_PG), %esi
    orl     $(MMU_CR0_WP), %esi
    movl    %esi, %cr0

//pushl $(VIRT_TO_PHYS_WO(entrypgdir))
//call __load_page_directory
//call __enable_paging
//popl %eax


//        push $(gdtdesc-KERN_BASE)
//        call gdt_flush
//call gdt_init


    lgdt    (gdtdesc)

    // The kernel uses DS, ES and SS.
    movw $__KERNEL_DS_SELECTOR, %si
    movw %si, %ds
    movw %si, %es
    movw %si, %ss

    // The kernel never uses FS or GS -> leave those set to zero
    movw $0, %si
    movw %si, %fs
    movw %si, %gs

    // Load the kernel text segment into CS with a long jump.
    ljmp $__KERNEL_CS_SELECTOR, $1f

1:

    # Set up kmain()'s arguments
    pushl $bootstrap_stack_top                          # Bootstrap stack top
    pushl %eax                          # Multiboot magic number
    pushl %ebx                          # Multiboot info

	/* Enter the high-level kernel. The ABI requires the stack is 16-byte aligned at the time of the call instruction
	 * (which afterwards pushes the return pointer of size 4 bytes). The stack was originally 16-byte aligned above and
	 * we've since pushed a multiple of 16 bytes to the stack since (pushed 0 bytes so far) and the alignment is thus
	 * preserved and the call is well defined.
	 */
//mov $kmain, %eax
//jmp *%eax

call kmain                  # Call kmain (defined in C), passing in the Multiboot info, Multiboot magic number and
                                # stack pointer (see /kernel/main.c) (see https://wiki.osdev.org/Calling_Conventions).

    # If we reach this point (kmain() returned), the best idea would be trying to halt the computer...

.hang:
	/* We need to do this inside an infinite loop because as soon as a Non-Maskable Interrupt (NMI) arrives, the CPU
	 * will restart!
	 */
    hlt
    jmp .hang


# Bootstrap GDT
.align 4
gdt_x:
SEG_NULL				# null seg
SEG(STA_X|STA_R, 0x0, 0xffffffff)	# code seg
SEG(STA_W, 0x0, 0xffffffff)	        # data seg

gdtdesc:
.word   0x17                            # sizeof(gdt) - 1
.long   (gdt_x)                            # address gdt
//.long   (gdt_x-KERN_BASE)                            # address gdt