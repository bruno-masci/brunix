OUTPUT_FORMAT("elf32-i386")
OUTPUT_ARCH("i386")
ENTRY(start)
phys = 0x00100000;      /* 1 MB */

SECTIONS {
    kernel_start = .;

    .boot : {
        /* ensure that the multiboot header is at the beginning */
        *(.multiboot_header)
    }

    .text ALIGN(4096) : AT(ADDR(.text)) {
	    *(.text)
    }
    .rodata ALIGN(4096) : AT(ADDR(.rodata)) {
        *(.rodata)
        *(.rodata.*)
    }
    .data ALIGN(4096) : AT(ADDR(.data)) {
    	*(.data)
    }
    bss_start = .;
        .bss ALIGN(4096) : AT(ADDR(.bss)) {
        *(.bss)
    }

    bss_end = .;
    kernel_end = .;
}