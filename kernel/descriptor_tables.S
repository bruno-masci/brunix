#
# Gdt.s -- contains global descriptor table and interrupt descriptor table
#          setup code.
#          Based on code from Bran's kernel development tutorials.
#          Rewritten for JamesM's kernel development tutorials.



#include <arch/x86/asm.h>



.globl __gdt_flush          # Allows the C code to call gdt_flush().
.type __gdt_flush, @function
__gdt_flush:
    mov 4(%esp), %eax  # Get the pointer to the GDT, passed as a parameter.
    lgdt (%eax)        # Load the new GDT pointer

    movl $__KERNEL_DS, %eax     # 0x10 is the offset in the GDT to our data segment
    movl %eax, %ds              # Load all data segment selectors
    movl %eax, %es
    movl %eax, %fs
    movl %eax, %gs
    movl %eax, %ss
    ljmp $__KERNEL_CS, $.flush   # The offset to our code segment in bytes (GDT). Far jump!
.flush:
    ret
