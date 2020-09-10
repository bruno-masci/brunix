


ENTRY(_start)

. = 
   (0x00000000 + 0x00100000)
            ;

SECTIONS {

    .boot : {

        *(.multiboot_header)
    }

    .init : {
        *(.init)
    }

    PROVIDE(kernel_start = .);

    .text ALIGN(4096) : AT (ADDR(.text) - 
                                         0x00000000
                                                  ) {
     *(.text .stub .text.* .gnu.linkonce.t.*)
    }
    PROVIDE(etext = .);

    .rodata ALIGN(4096) : AT (ADDR(.rodata) - 
                                             0x00000000
                                                      ) {
        *(.rodata .rodata.* .gnu.linkonce.r.*)
    }




 PROVIDE(data = .);

    .data ALIGN(4096) : AT (ADDR(.data) - 
                                         0x00000000
                                                  ) {
     *(.data)
    }

 PROVIDE(edata = .);

    .bss ALIGN(4096) : AT (ADDR(.bss) - 
                                       0x00000000
                                                ) {
        *(.bss)
    }

 PROVIDE(kernel_end = .);

    /DISCARD/ : {
        *(.eh_frame .note.GNU-stack)
    }
}
