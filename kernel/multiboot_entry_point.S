/**
 * @author Bruno Masci
 * @brief Multiboot entry point
 * @see /linker.ld.pp
 *
 * The kernel starts executing in this file.
 */


#include <arch/x86/multiboot.h>
#include <arch/x86/memlayout.h>


///////////////////////////////////////////////////////////////////////////////
// ********************* SECTION: multiboot_header ************************* //
///////////////////////////////////////////////////////////////////////////////

/* Here we declare the Multiboot header that marks the program as a kernel. The bootloader will search for this header in
 * the first 8 KiB of the kernel file. This header must be longword (32-bit) aligned (".align 4"), and it is in its own
 * section so we can put it at the beginning of the kernel image file (see "linker.ld.pp" file).
 * Note that we need to force the flag "a" (allocatable) because this is not a default text section (like .text or .rodata).
 */
.section .multiboot_header, "a"
.align 4
    .long MBOOT_HEADER_MAGIC
    .long MBOOT_HEADER_FLAGS
    .long MBOOT_HEADER_CHECKSUM



///////////////////////////////////////////////////////////////////////////////
// *************************** SECTION: init ******************************* //
///////////////////////////////////////////////////////////////////////////////

// this sections has (LA 1 MiB - VA 1 MiB) + multiboot size
.section .init, "ax"

# Entering kernel, with paging off and interrupts disabled (by the bootloader).
.globl _start
_start:
# By convention, the _start symbol specifies the ELF entry point.
# Given that GRUB needs the virtual address of the entry point and the physical and virtual address spaces does not match
# anymore, we can get rid of the original entry point and reuse the '_start' symbol name.


    /* The bootloader has loaded us into 32-bit protected mode on a x86 machine.
     * Interrupts are disabled. Paging is disabled. The processor state is as defined in the Multiboot standard. The kernel
     * has full control of the CPU. The kernel can only make use of hardware features and any code it provides as part of
     * itself. There's no printf function, unless the kernel provides its own <stdio.h> header and a printf implementation.
     */

    /*
     * At this point, the Multiboot-compliant bootloader should have set:
     *      %eax -> Multiboot magic number (see "/include/arch/x86/multiboot.h")
     *      %ebx -> Multiboot information structure's 32-bit *physical* address
     */




///////////////////////////////////////////////////////////////////////////////
// *************************** SECTION: text ******************************* //
///////////////////////////////////////////////////////////////////////////////

# The linker script (see "linker.ld.pp" file) specifies _start as the entry point to the kernel and the bootloader will
# jump to this position once the kernel has been loaded. It doesn't make sense to return from this function as the
# bootloader is gone.
.section .text

    # We are not doing any meaningful action right now, so let's halt the computer.
    jmp .hang

.hang:
	/* We need to do this inside an infinite loop because as soon as a non-maskable interrupt (NMI) arrives
	 * (see https://wiki.osdev.org/Non_Maskable_Interrupt) the CPU will restart!
	 */
    hlt
    jmp .hang
.end:
