//#include <arch/x86/multiboot.h>
#include <arch/x86/mmu.h>
#include <arch/x86/memlayout.h>
//#include <arch/x86/asm.h>

.section .text

.globl __load_page_directory      # Allows the C code to call __load_page_directory().
.type __load_page_directory, @function
__load_page_directory:
    pushl %ebp
    movl %esp, %ebp
    movl 8(%ebp), %eax
    movl %eax, %cr3

    movl %ebp, %esp
    popl %ebp
    ret

.globl __enable_paging            # Allows the C code to call __enable_paging().
.type __enable_paging, @function
__enable_paging:
    # function prologue
    pushl %ebp
    movl %esp, %ebp

    # Turn on page size extension for 4Mbyte pages
    #movl    %cr4, %eax
    #orl     $MMU_CR4_PSE, %eax
    #movl    %eax, %cr4

    # Turn on paging
    movl    %cr0, %eax
    orl     $(MMU_CR0_PG | MMU_CR0_WP), %eax

    movl    %eax, %cr0

    # function epilogue
    movl %ebp, %esp
    popl %ebp
    ret