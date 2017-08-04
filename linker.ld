ENTRY(_start)
. = 0x00100000;

SECTIONS {
    kernel_start = .;

    boot_start = .;
    .boot : {
        /* ensure that the multiboot header is at the beginning */
        *(.multiboot_header)
    }
    boot_end = .;

    text_start = .;
    .text ALIGN(4096) : AT(ADDR(.text)) {
	    *(.text)
    }
    text_end = .;

    data_start = .;
    .rodata ALIGN(4096) : AT(ADDR(.rodata)) {
        *(.rodata)
        *(.rodata.*)
    }
    .data ALIGN(4096) : AT(ADDR(.data)) {
    	*(.data)
    }
    data_end = .;

    bss_start = .;
        .bss ALIGN(4096) : AT(ADDR(.bss)) {
        *(.bss)
    }
    bss_end = .;

    kernel_end = .;
}
