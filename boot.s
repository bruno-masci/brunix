global start

section .text
bits 32



; This is the kernel''''''s entry point. We could either call main here,
; or we can use this to setup the stack or other nice stuff, like
; perhaps setting up the GDT and segments. Please note that interrupts
; are disabled at this point: More on interrupts later!
align 4
start:
	mov esp, default_stack_pointer	; initialize stack pointer.

	extern kmain            ; the function kmain is defined in C
	push ebx
	push eax
	call kmain              ; call kmail, passing in the magic number and multiboot info
	cli                     ; stop interrupts
	;hlt2 ; halt the CPU

.loop:
    jmp .loop                   ; loop forever




KERNEL_STACK_SIZE equ 8192                 ; size of stack in bytes
section .bss
align 4
	resb KERNEL_STACK_SIZE         	   ; This reserves 8KBytes of memory here
default_stack_pointer:			   ; The stack actually grows downwards

;TODO Ver cpu_init etc de entry.asm eduOS
