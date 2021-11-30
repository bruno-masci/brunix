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
#include <brunix/linkage.h>


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


.align 16
user_test_stack_bottom:
    .space BOOTSTRAP_STACK_SIZE
user_test_stack_top:


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


    jmp setup_early_paging

early_paging_ready:

    # Clear the frame pointer register (EBP) so that once we get into debugging C code, stack backtraces will be
    # terminated properly.
	movl $0x0, %ebp			# nuke frame pointer

    movl $bootstrap_stack_top, %esp     # Set up a stack, setting %esp register to point to the top of our stack (as it
                                        # grows downwards on x86 systems). This is necessarily done in assembly as
                                        # languages such as C cannot function without a stack.


   movl %eax, %edi
   call gdt_init
   movl %edi, %eax


    lgdt    (gdt_ptr)

    movw $__TSS_SEGMENT_SELECTOR, %dx
    ltr %dx

    call setup_all_segments_but_CS

    // Load the kernel text segment into CS with a long jump.
    ljmp $__KERNEL_CS_SELECTOR, $1f
//TODO otras formas de ljmp:   ljmpl    $(SEG_KCODE<<3), $(start32)    o    mov $main, %eax; jmp *%eax
1:



#TODO set IDT here??



    # Set up start_kernel()'s arguments
    pushl $bootstrap_stack_top              # Bootstrap stack top
    pushl %eax                              # Multiboot magic number
    pushl %ebx                              # Multiboot info

	/* Enter the high-level kernel. The ABI requires the stack is 16-byte aligned at the time of the call instruction
	 * (which afterwards pushes the return pointer of size 4 bytes). The stack was originally 16-byte aligned above and
	 * we've since pushed a multiple of 16 bytes to the stack since (pushed 0 bytes so far) and the alignment is thus
	 * preserved and the call is well defined.
	 */
//mov $start_kernel, %eax
//jmp *%eax

    call start_kernel           # Call kmain (defined in C), passing in the Multiboot info, Multiboot magic number and
                                # stack pointer (see /kernel/main.c) (see https://wiki.osdev.org/Calling_Conventions).

    # If we reach this point (start_kernel() returned), the best idea would be trying to halt the computer...

.hang:
	/* We need to do this inside an infinite loop because as soon as a Non-Maskable Interrupt (NMI) arrives, the CPU
	 * will restart!
	 */
    hlt
    jmp .hang




.globl jump_usermode
#.extern test_user_function
jump_usermode:
    cli

	movw $(__USER_DS_SELECTOR | 3), %ax # ring 3 data with bottom 2 bits set for ring 3
	movw %ax, %ds
	movw %ax, %es
	#mov %ax, %fs
	#mov %ax, %gs # SS is handled by iret

	# set up the stack frame iret expects
#	mov %esp, %eax
	push $(__USER_DS_SELECTOR | 3) # data selector
	push $user_test_stack_top # current esp
	pushf # eflags
	push $(__USER_CS_SELECTOR | 3) # code selector (ring 3 code with bottom 2 bits set for ring 3)
	#FIXME push $test_user_function # instruction address to return to

	sti

	iret


# We don't use %eax/%ax in order to preserve the value stored on %eax by the bootloader; we use %esi/%si instead.

/* Identity-map the kernel in low 4MB memory for ease of transition */

setup_early_paging:
    # Turn on page size extension for 4Mbyte pages
    movl    %cr4, %esi
    orl     $(MMU_CR4_PSE), %esi
    movl    %esi, %cr4
    # Set page directory
    movl    $(VIRT_TO_PHYS_WO(entrypgdir)), %esi
    movl    %esi, %cr3
    # Turn on paging.
    movl    %cr0, %esi
    orl     $(MMU_CR0_PG | MMU_CR0_WP), %esi
    movl    %esi, %cr0
    jmp early_paging_ready
#    ret			/* this also flushes the prefetch-queue */  TAKEN FROM LINUX

//pushl $(VIRT_TO_PHYS_WO(entrypgdir))
//call __load_page_directory
//call __enable_paging
//popl %eax

.globl __load_page_directory
.type __load_page_directory, @function
__load_page_directory:
    pushl %ebp
    movl %esp, %ebp

    movl 8(%ebp), %eax      # get page dir's physical address
    movl %eax, %cr3

    movl %ebp, %esp
    popl %ebp
    ret



setup_all_segments_but_CS:
    // The kernel uses DS, ES and SS.
    movw $__KERNEL_DS_SELECTOR, %si
    movw %si, %ds
    movw %si, %es
    movw %si, %ss

    // The kernel never uses FS or GS -> leave those set to zero
    movw $0, %si
    movw %si, %fs
    movw %si, %gs
    ret
