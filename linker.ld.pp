/**
 * @author Bruno Masci
 * @brief Linker script for the kernel
 * @see /kernel/multiboot_entry_point.S
 * @see http://wiki.osdev.org/Multiboot
 * @see http://wiki.osdev.org/LD
 * @see http://wiki.osdev.org/GCC_Cross-Compiler
 * @see https://wiki.osdev.org/ELF
 *
 * This file is actually a template for the linker script. We apply the C preprocessor on this file so we can include
 * some constants (instead of duplicating them) and use C-style comments.
 * The ".pp" suffix in this file name stands for "PreProcessor".
 */


#include <arch/x86/memlayout.h>     // for KERN_LINK


/* >>>>> DISCLAIMER:
 * We don't specify neither OUTPUT_FORMAT nor OUTPUT_ARCH commands here (see http://wiki.osdev.org/GCC_Cross-Compiler)
 */

// This declares the symbol where kernel execution begin (see /kernel/multiboot_entry_point.S file)
ENTRY(_start)

// "." means the current *virtual* address
. = KERN_LINK;

SECTIONS {

    .boot ALIGN(4096) : {
        // Ensure that the multiboot header is at the beginning of the generated image
        *(.multiboot_header)
    }

    .text ALIGN(4096) : {
	    *(.text)
    }
}