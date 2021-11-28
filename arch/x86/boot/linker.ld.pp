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


#define __ASSEMBLER__   // trick to avoid typedef, etc.
#include <asm/memlayout.h>     // for KERN_LINK, KERN_BASE
#undef __ASSEMBLER__


/* >>>>> DISCLAIMER:
 * We don't specify neither OUTPUT_FORMAT nor OUTPUT_ARCH commands here (see http://wiki.osdev.org/GCC_Cross-Compiler)
 */

// This declares the symbol where kernel execution begin (see /kernel/multiboot_entry_point.S file)
ENTRY(_start)

// "." means the current *virtual* address
. = KERN_LINK;

SECTIONS {

    .boot : AT (EXT_MEM_BASE) {
        // Ensure that the multiboot header is at the beginning of the generated image
        *(.multiboot_header)
    }

    PROVIDE(kernel_start = .);

    .text : {//AT (0x10000c) {
	    *(.text .stub)
    }
    PROVIDE(etext = .);

    .rodata : {//AT (ADDR(.rodata) - KERN_BASE) {
        *(.rodata)
    }

    /* Include debugging information in kernel memory */
    .stab : {//AT (ADDR(.stab) - KERN_BASE) {
        PROVIDE(__STAB_BEGIN__ = .);
        *(.stab);
        PROVIDE(__STAB_END__ = .);
        BYTE(0)     /* Force the linker to allocate space for this section */
    }

    .stabstr : {//AT (ADDR(.stabstr) - KERN_BASE) {
        PROVIDE(__STABSTR_BEGIN__ = .);
        *(.stabstr);
        PROVIDE(__STABSTR_END__ = .);
        BYTE(0)     /* Force the linker to allocate space for this section */
    }

    /* Conventionally, Unix linkers provide pseudo-symbols etext, edata, and end, at the end of the text, data, and bss.
	 * For the kernel mapping, we need the address at the beginning of the data section, but that's not one of the
     * conventional symbols (the convention started before there was a read-only rodata section between text and data). */
	PROVIDE(data = .);

    .data : {//AT (ADDR(.data) - KERN_BASE) {
    	*(.data)
    }

	PROVIDE(edata = .);

    .bss : {//AT (ADDR(.bss) - KERN_BASE) {
        *(.bss)
    }

	PROVIDE(kernel_end = .);

    // https://stackoverflow.com/questions/26300819/why-gcc-compiled-c-program-needs-eh-frame-section#26302715

    /DISCARD/ : {
        *(.eh_frame .note.GNU-stack)
    }
}
