#
# Gdt.s -- contains global descriptor table and interrupt descriptor table
#          setup code.



#include <arch/x86/asm.h>



.globl __gdt_flush
#.type __gdt_flush, @function
__gdt_flush:
    pushl %ebp
    movl %esp, %ebp
    subl $4, %esp
    movl 8(%ebp), %eax  # Get the pointer to the GDT, passed as a parameter.
    lgdt (%eax)        # Load the new GDT pointer

    movl $__KERNEL_DS, %eax     # 0x10 is the offset in the GDT to our data segment
    movl %eax, %ds              # Load all data segment selectors
    movl %eax, %es
    movl %eax, %fs
    movl %eax, %gs
    movl %eax, %ss
    ljmp $__KERNEL_CS, $.flush   # The offset to our code segment in bytes (GDT). Far jump!
.flush:
    movl %ebp, %esp
    popl %ebp
    ret


.globl __idt_flush    # Allows the C code to call __idt_flush().
.type __idt_flush, @function
__idt_flush:
    pushl %ebp
    movl %esp, %ebp
    mov 8(%ebp), %eax  # Get the pointer to the IDT, passed as a parameter.
    lidt (%eax)        # Load the IDT pointer.
    ret