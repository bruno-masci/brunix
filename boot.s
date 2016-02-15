global start

section .text
bits 32



; This is the kernel''''''s entry point. We could either call main here,
; or we can use this to setup the stack or other nice stuff, like
; perhaps setting up the GDT and segments. Please note that interrupts
; are disabled at this point: More on interrupts later!
start:
	;mov esp, stack_top

    ; print `OK` to screen
    ;mov dword [0xb8000], 0x2f4b2f4f

    extern kmain            ; the function kmain is defined elsewhere
    ;push dword 3            ; arg3
    ;push dword 2            ; arg2
    ;push dword 1            ; arg1
    call kmain              ; call the function, the result will be in eax
 ;cli  ; stop interrupts
 ;hlt2 ; halt the CPU

.loop:
    jmp .loop                   ; loop forever






; Here is the definition of our BSS section. Right now, we'll use
; it just to store the stack. Remember that a stack actually grows
; downwards, so we declare the size of the data before declaring
; the identifier 'stack_top'
;section .bss
;stack_bottom:
;    resb 64
;stack_top:



;sacado de little book
;KERNEL_STACK_SIZE equ 4096                 ; size of stack in bytes
;section .bss:
;align 4                                    ; align at 4 bytes
;kernel_stack:                              ; label points to beginning of memory
;resb KERNEL_STACK_SIZE                     ; reserve stack for the kernel

;mov esp, kernel_stack + KERNEL_STACK_SIZE  ; point esp to the start of the
                                            ; stack (end of memory area)
