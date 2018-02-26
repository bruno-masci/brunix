; (see https://www.gnu.org/software/grub/manual/multiboot/multiboot.html)


MBOOT_HEADER_MAGIC          equ 0x1BADB002
MBOOT_HEADER_FLAGS          equ 0x00000002 ; Include info on available memory.
MBOOT_HEADER_CHECKSUM       equ -(MBOOT_HEADER_MAGIC + MBOOT_HEADER_FLAGS)

; Here we declare the Multiboot header that marks the program as a kernel. The
; bootloader will search for this header in the first 8 KiB of the kernel file.
; This header must be longword (32-bit) aligned ("align 4"), and it is in its
; own section so we can put it at the beginning of the kernel image file (see
; "linker.ld" file).
section .multiboot_header
align 4
    dd  MBOOT_HEADER_MAGIC
    dd  MBOOT_HEADER_FLAGS
    dd  MBOOT_HEADER_CHECKSUM


; The Multiboot standard doesn't define a value for the stack pointer register
; (esp) so it is up to the kernel to provide a stack.
; Here we allocate room for a small temporal stack by creating a symbol at the
; bottom of it, then allocating BOOTSTRAP_STACK_SIZE bytes for it, and finally
; creating a symbol at the top of it. Please note that the stack grows
; downwards on x86.
; This stack is in its own section so it can be marked 'nobits', which means
; the kernel file is smaller because it doesn't contain an uninitialized stack.
; The stack on x86/GCC must be 16-byte aligned according to the System V ABI
; standard (see https://wiki.osdev.org/System_V_ABI). The compiler will assume
; the stack is properly aligned and failure to align the stack will result in
; undefined behavior.
BOOTSTRAP_STACK_SIZE equ 16384  ; Size of stack in bytes (16 KiB).

section .bss nobits
align 16
stack_bottom:
    resb BOOTSTRAP_STACK_SIZE
stack_top:


; The linker script (see "linker.ld" file) specifies _start as the entry point
; to the kernel and the bootloader will jump to this position once the kernel
; has been loaded. It doesn't make sense to return from this function as the
; bootloader is gone.
section .text
global _start

_start:
    ; The bootloader has loaded us into 32-bit protected mode on a x86 machine.
    ; Interrupts are disabled. Paging is disabled. The processor state is as
    ; defined in the Multiboot standard. The kernel has full control of the
    ; CPU. The kernel can only make use of hardware features and any code it
    ; provides as part of itself. There's no printf function, unless the kernel
    ; provides its own <stdio.h> header and a printf implementation.

	; To set up a stack, we set the esp register to point to the top of our
	; stack (as it grows downwards on x86 systems). This is necessarily done
	; in assembly as languages such as C cannot function without a stack.
	mov esp, stack_top


	; Enter the high-level kernel. The ABI requires the stack is 16-byte
	; aligned at the time of the call instruction (which afterwards pushes the
	; return pointer of size 4 bytes). The stack was originally 16-byte aligned
	; above and we've since pushed a multiple of 16 bytes to the stack since
	; (pushed 0 bytes so far) and the alignment is thus preserved and the call
	; is well defined.
	extern kmain        ; The function kmain is defined in C.
	push esp            ; Save initial stack pointer.
	push eax            ; Tell the OS it was loaded by a Multiboot-compliant
	                    ; bootloader (see include/arch/x86/multiboot.h and
                        ; kernel/main.c)
	push ebx            ; Save the 32-bit physical address of the Multiboot
	                    ; information structure provided by the boot loader.
	call kmain          ; Call kmail, passing in the multiboot info, magic
	                    ; number and stack pointer (see kernel/main.c) (see
	                    ; https://wiki.osdev.org/Calling_Conventions).

	; If the system has nothing more to do, put the computer into an infinite
	; loop. To do that:
    ; 1) Disable interrupts with 'cli' (see https://wiki.osdev.org/Interrupts).
    ;    Please note that at this point kmain() has enabled interrupts, so we
    ;    need to disable them. This applies only to maskable interrupts.
    ; 2) Wait for the next interrupt to arrive with 'hlt' (halt instruction).
    ;    This will stop the CPU, but as soon as a non-maskable interrupt (NMI)
    ;    arrives (see https://wiki.osdev.org/Non_Maskable_Interrupt) the CPU
    ;    will restart, so we need a way to keep halting all the time ('jmp') :P
	cli
.hang:
    hlt
    jmp .hang
.end: