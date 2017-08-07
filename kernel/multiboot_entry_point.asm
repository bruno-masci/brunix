; See Multiboot standard
;TODO Ver http://download-mirror.savannah.gnu.org/releases/grub/phcoder/multiboot.pdf


MULTIBOOT_HEADER_MAGIC          equ 0x1BADB002
MULTIBOOT_HEADER_FLAGS          equ 0x00000007
MULTIBOOT_HEADER_CHECKSUM       equ -(MULTIBOOT_HEADER_MAGIC + MULTIBOOT_HEADER_FLAGS)

; Here we declare a multiboot header that marks the program as a kernel. The
; bootloader will search for this signature in the first 8 KiB of the kernel
; file, aligned at a 32-bit boundary ("align 4"). The signature is in its own
; section so the header can be forced to be within the first 8 KiB of the
; kernel file (see "linker.ld" file).
section .multiboot_header
align 4
    dd  MULTIBOOT_HEADER_MAGIC
    dd  MULTIBOOT_HEADER_FLAGS
    dd  MULTIBOOT_HEADER_CHECKSUM

; TODO: Revisar esto de abajo
    ; aout kludge (unused)
    dd 0,0,0,0,0

    ; Video mode
    dd  1           ; Linear graphics please?
    dd  0           ; Preferred width
    dd  0           ; Preferred height
    dd  32          ; Preferred pixel depth






; The multiboot standard does not define the value of the stack pointer register
; (esp) and it is up to the kernel to provide a stack.
; This allocates room for a small temporal stack by creating a symbol at the
; bottom of it, then allocating BOOTSTRAP_STACK_SIZE bytes for it, and finally
; creating a symbol at the top. The stack grows downwards on x86.
; This stack is in its own section so it can be marked nobits, which means the
; kernel file is smaller because it does not contain an uninitialized stack.
; The stack on x86 must be 16-byte aligned according to the System V ABI
; standard. The compiler will assume the stack is properly aligned and failure
; to align the stack will result in undefined behavior.
BOOTSTRAP_STACK_SIZE equ 14384     ; size of stack in bytes (16 KiB)

section .bss nobits ; TODO revisar! parece que .bss por default es nobits
align 4
stack_bottom:
    resb BOOTSTRAP_STACK_SIZE
stack_top:







; The linker script (see "linker.ld" file) specifies _start as the entry point
; to the kernel and the bootloader will jump to this position once the kernel
; has been loaded. It doesn't make sense to return from this function as the
; bootloader is gone.
section .text

; Declare _start as a function symbol with the given symbol size.
;global _start:function (_start.end - _start)
global _start
# By convention, the _start symbol specifies the ELF entry point.
# Since we haven't set up virtual memory yet, our entry point is
# the physical address of 'entry'.

_start:
    ; The bootloader has loaded us into 32-bit protected mode on a x86 machine.
    ; Interrupts are disabled. Paging is disabled. The processor state is as
    ; defined in the multiboot standard.

	; To set up a stack, we set the esp register to point to the top of our
	; stack (as it grows downwards on x86 systems). This is necessarily done
	; in assembly as languages such as C cannot function without a stack.
	mov esp, stack_top

    ; This is a good place to initialize crucial processor state before the
	; high-level kernel is entered. Note that the processor is not fully
	; initialized yet: Features such as floating point instructions and
	; instruction set extensions are not initialized yet. The GDT should be
	; loaded here. Paging should be enabled here. C++ features such as global
	; constructors and exceptions will require runtime support to work as well.

	; Enter the high-level kernel. The ABI requires the stack is 16-byte
	; aligned at the time of the call instruction (which afterwards pushes
	; the return pointer of size 4 bytes).
	;TODO comentar un poco mejor esto de abajo
	extern kmain            ; the function kmain is defined in C
	push esp				; Save initial stack pointer
	push eax
	push ebx
	call kmain              ; call kmail, passing in the multiboot info, magic number and stack pointer

	; If the system has nothing more to do, put the computer into an
    ; infinite loop. To do that:
    ; 1) Disable interrupts with cli (clear interrupt enable in eflags).
    ;    They are already disabled by the bootloader, so this is not needed.
    ;    Mind that you might later enable interrupts and return from
    ;    kernel_main (which is sort of nonsensical to do).
    ; 2) Wait for the next interrupt to arrive with hlt (halt instruction).
    ;    Since they are disabled, this will lock up the computer.
    ; 3) Jump to the hlt instruction if it ever wakes up due to a
    ;    non-maskable interrupt occurring or due to system management mode.
	cli
.hang:
    hlt
    jmp .hang
.end: