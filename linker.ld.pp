/**
 * @author Bruno Masci
 * @brief Simple linker script for the kernel
 * @see /kernel/multiboot_entry_point.S
 * @see http://wiki.osdev.org/Multiboot
 * @see http://wiki.osdev.org/LD
 * @see http://wiki.osdev.org/GCC_Cross-Compiler
 *
 * This file is actually a template for the linker script. We apply the C preprocessor on this file so we can include
 * some constants (instead of duplicating them) and use C-style comments.
 * The ".pp" suffix in this file name stands for "PreProcessor".
 */


#include <arch/x86/memlayout.h>     // for KERN_LINK, KERN_BASE


/* <<< DISCLAIMER >>>
 * We don't specify neither OUTPUT_FORMAT nor OUTPUT_ARCH commands here (see http://wiki.osdev.org/GCC_Cross-Compiler)
 */


// This declares the symbol where kernel execution begin (see /kernel/multiboot_entry_point.S file)
ENTRY(_start)

// "." means the current address
. = KERN_LINK;

SECTIONS {

    .boot : {
        // Ensure that the multiboot header is at the beginning of the generated image
        *(.multiboot_header)
    }

    .init : {
        *(.init)
    }

    .text ALIGN(4096) : AT (ADDR(.text) - KERN_BASE) {
	    *(.text .stub .text.* .gnu.linkonce.t.*)
    }

    .rodata ALIGN(4096) : AT (ADDR(.rodata) - KERN_BASE) {
        *(.rodata .rodata.* .gnu.linkonce.r.*)
    }

    /* Conventionally, Unix linkers provide pseudo-symbols etext, edata, and end, at the end of the text, data, and bss.
	 * For the kernel mapping, we need the address at the beginning of the data section, but that's not one of the
     * conventional symbols (the convention started before there was a read-only rodata section between text and data). */
	PROVIDE(data = .);

    .data ALIGN(4096) : AT (ADDR(.data) - KERN_BASE) {
    	*(.data)
    }

    .bss ALIGN(4096) : AT (ADDR(.bss) - KERN_BASE) {
        *(.bss)
    }

	PROVIDE(kernel_end = .);

    /DISCARD/ : {
        *(.eh_frame .note.GNU-stack)
    }
}