;
; Gdt.s -- contains global descriptor table and interrupt descriptor table
;          setup code.
;          Based on code from Bran's kernel development tutorials.
;          Rewritten for JamesM's kernel development tutorials.



%define __KERNEL_CS 0x08
%define __KERNEL_DS 0x10


[GLOBAL gdt_flush]          ; Allows the C code to call gdt_flush().

gdt_flush:
    mov eax, [esp+4]        ; Get the pointer to the GDT, passed as a parameter.
    lgdt [eax]              ; Load the new GDT pointer

    mov ax, __KERNEL_DS     ; 0x10 is the offset in the GDT to our data segment
    mov ds, ax              ; Load all data segment selectors
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    jmp __KERNEL_CS:.flush   ; The offset to our code segment in bytes (GDT). Far jump!
.flush:
    ret

[GLOBAL idt_flush]          ; Allows the C code to call idt_flush().

idt_flush:
    mov eax, [esp+4]        ; Get the pointer to the IDT, passed as a parameter.
    lidt [eax]              ; Load the IDT pointer.
    ret