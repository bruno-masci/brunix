/* Note that we don't specify neither OUTPUT_FORMAT nor OUTPUT_ARCH commands
 * here (see http://wiki.osdev.org/GCC_Cross-Compiler)
 */

/* This declares the symbol where kernel execution begin */
ENTRY(_start)

/* Here we specify that we want our kernel loaded at 1 MiB in physical memory
 * to avoid the "low memory" area (see http://wiki.osdev.org/Memory_Map_(x86))
 * REMEMBER TO CHANGE 'EXT_MEM_BASE' constant in "include/arch/x86/memlayout.h" ACCORDING TO THIS VALUE!
 */
. = 0x00100000;

SECTIONS {
    kernel_start = .;

    .boot : {
        /* ensure that the multiboot header is at the beginning */
        *(.multiboot_header)
    }
    .text ALIGN(4096) : {
	    *(.text)
    }
    .rodata ALIGN(4096) : {
        *(.rodata)
        *(.rodata.*)
    }
    .data ALIGN(4096) : {
    	*(.data)
    }
    .bss ALIGN(4096) : {
        *(.bss)
    }

    kernel_end = .;

    /DISCARD/ : {
        *(.eh_frame .note.GNU-stack)
    }
}