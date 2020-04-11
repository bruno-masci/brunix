/* Simple linker script for the kernel.
 * See the GNU ld manual to learn the syntax.
 */

/* Note that we don't specify neither OUTPUT_FORMAT nor OUTPUT_ARCH commands
 * here (see http://wiki.osdev.org/GCC_Cross-Compiler)
 */

/* This declares the symbol where kernel execution begin */
ENTRY(_start)

/* Here we specify that we want our kernel loaded at 1 MiB in physical memory
 * to avoid the "low memory" area (see http://wiki.osdev.org/Memory_Map_(x86))
 */
. = 0x00100000;     /* "." means the current address */
                    /* See KERN_LINK, declared in "include/arch/x86/memlayout.h" */


SECTIONS {
    PROVIDE(kernel_start = .);

    .boot : {
        /* Ensure that the multiboot header is at the beginning */
        *(.multiboot_header)
    }
    .text ALIGN(4096) : {
	    *(.text .stub .text.* .gnu.linkonce.t.*)
    }
    PROVIDE(etext = .);

    .rodata ALIGN(4096) : {
        *(.rodata .rodata.* .gnu.linkonce.r.*)
    }

	/* Conventionally, Unix linkers provide pseudo-symbols
	 * etext, edata, and end, at the end of the text, data, and bss.
	 * For the kernel mapping, we need the address at the beginning
	 * of the data section, but that's not one of the conventional
	 * symbols, because the convention started before there was a
	 * read-only rodata section between text and data. */
	PROVIDE(data = .);

    .data ALIGN(4096) : {
    	*(.data)
    }

	PROVIDE(edata = .);

    .bss ALIGN(4096) : {
        *(.bss)
    }

	PROVIDE(kernel_end = .);

    /DISCARD/ : {
        *(.eh_frame .note.GNU-stack)
    }
}