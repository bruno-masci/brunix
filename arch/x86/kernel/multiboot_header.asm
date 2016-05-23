section .multiboot_header


MULTIBOOT_HEADER_MAGIC          equ 0x1BADB002
MULTIBOOT_HEADER_FLAGS          equ 0x00000007
MULTIBOOT_HEADER_CHECKSUM       equ -(MULTIBOOT_HEADER_MAGIC + MULTIBOOT_HEADER_FLAGS) & 0xFFFFFFFF

align 16, db 0
multiboot_header:
    dd  MULTIBOOT_HEADER_MAGIC
    dd  MULTIBOOT_HEADER_FLAGS
    dd  MULTIBOOT_HEADER_CHECKSUM

    ; aout kludge (unused)
    dd 0,0,0,0,0

    ; Video mode
    dd  1           ; Linear graphics please?
    dd  0           ; Preferred width
    dd  0           ; Preferred height
    dd  32          ; Preferred pixel depth

;TODO Ver http://download-mirror.savannah.gnu.org/releases/grub/phcoder/multiboot.pdf
