/* Simple linker script for the kernel.
 * See the GNU ld manual to learn the syntax.
 */

/* Note that we don't specify neither OUTPUT_FORMAT nor OUTPUT_ARCH commands
 * here (see http://wiki.osdev.org/GCC_Cross-Compiler)
 */

#include <arch/x86/memlayout.h>     // for KERN_LINK, KERN_BASE

/* This declares the symbol where kernel execution begin */
/* ahora este simbolo se refiere a la dir virtual higher half */
ENTRY(_start)

. = KERN_LINK;     /* "." means the current address */

SECTIONS {

    .boot : {
        /* Ensure that the multiboot header is at the beginning */
        *(.multiboot_header)
    }

    .init : {
        *(.init)
    }

    PROVIDE(kernel_start = .);

    .text ALIGN(4096) : AT (ADDR(.text) - KERN_BASE) {
	    *(.text .stub .text.* .gnu.linkonce.t.*)
    }
    PROVIDE(etext = .);

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

	PROVIDE(edata = .);

    .bss ALIGN(4096) : AT (ADDR(.bss) - KERN_BASE) {
        *(.bss)
    }

	PROVIDE(kernel_end = .);

    /DISCARD/ : {
        *(.eh_frame .note.GNU-stack)
    }
}