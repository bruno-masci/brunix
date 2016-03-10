global start

section .text
bits 32



; This is the kernel''''''s entry point. Please note that interrupts
; are disabled at this point!
align 4
start:
	mov esp, default_stack_pointer	; initialize stack pointer.

	extern kmain            ; the function kmain is defined in C
	push esp				;Save initial stack pointer (iria arriba?)
	push ebx
	push eax
	call kmain              ; call kmail, passing in the magic number and multiboot info
	cli                     ; stop interrupts
	jmp $					;Hang when this ends




KERNEL_STACK_SIZE equ 8192                 ; size of stack in bytes
section .bss
align 4
	resb KERNEL_STACK_SIZE         	   ; This reserves 8KBytes of memory here
default_stack_pointer:			   ; The stack actually grows downwards

;TODO Ver cpu_init etc de entry.asm eduOS