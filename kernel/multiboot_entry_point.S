/**
 * @author Bruno Masci
 * @brief Multiboot entry point
 * @see /linker.ld.pp
 * @see https://wiki.osdev.org/Interrupts
 * @see https://wiki.osdev.org/Non_Maskable_Interrupt
 *
 * The kernel starts executing in this file.
 */


#include <arch/x86/multiboot.h>     // for BOOT_HEADER_MAGIC, MBOOT_HEADER_FLAGS, MBOOT_HEADER_CHECKSUM


///////////////////////////////////////////////////////////////////////////////
// ********************* SECTION: multiboot_header ************************* //
///////////////////////////////////////////////////////////////////////////////

/* Here we declare the Multiboot header that marks the program as a kernel. The bootloader will search for this header in
 * the first 8 KiB of the kernel file. This header must be longword (32-bit) aligned (".align 4"), and it is in its own
 * section so we can put it at the beginning of the kernel image file (see "linker.ld.pp" file).
 * Note that we need to explicitly set the flag "a" (allocatable) because this is not a default text section (like .text or .rodata).
 */
.section .multiboot_header, "a"

.align 4
    .long MBOOT_HEADER_MAGIC
    .long MBOOT_HEADER_FLAGS
    .long MBOOT_HEADER_CHECKSUM



///////////////////////////////////////////////////////////////////////////////
// *************************** SECTION: text ******************************* //
///////////////////////////////////////////////////////////////////////////////

.section .text

# The linker script (see "linker.ld.pp" file) specifies _start as the entry point to the kernel and the bootloader will
# jump to this position once the kernel has been loaded. It doesn't make sense to return from here as the bootloader
# is gone.

# By convention, the _start symbol specifies the ELF entry point.
.globl _start
_start:

    /* The bootloader has loaded us into 32-bit Protected Mode. Entering kernel...
     * Interrupts are disabled (by the bootloader). Paging is disabled. The processor state is as defined in the Multiboot
     * standard. The kernel has full control over the CPU. The kernel can only make use of hardware features and any code
     * it provides as part of itself. There's no printf function, unless the kernel provides its own <stdio.h> header and
     * a printf() implementation (more on this later).
     *
     * At this point, the Multiboot-compliant bootloader should have set:
     *      %eax -> Multiboot magic number (see "/include/arch/x86/multiboot.h")
     *      %ebx -> Multiboot information structure's 32-bit *physical* address
     */

    # We are not doing any meaningful action right now, so let's halt the computer.
    jmp .hang

.hang:
	/* We need to do this inside an infinite loop because as soon as a Non-Maskable Interrupt (NMI) arrives, the CPU
	 * will restart!
	 */
    hlt
    jmp .hang
