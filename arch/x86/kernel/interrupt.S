/* TODO ver esto
    9.2.4  MOV or POP to SS Masks Some Interrupts and Exceptions
   Software that needs to change stack segments often uses a pair of
   instructions; for example:
     MOV SS, AX
     MOV ESP, StackTop

   If an interrupt or exception is processed after SS has been changed but
   before ESP has received the corresponding change, the two parts of the stack
   pointer SS:ESP are inconsistent for the duration of the interrupt handler or
   exception handler.
   To prevent this situation, the 80386, after both a MOV to SS and a POP to
   SS instruction, inhibits NMI, INTR, debug exceptions, and single-step traps
   at the instruction boundary following the instruction that changes SS. Some
   exceptions may still occur; namely, page fault and general protection fault.
   Always use the 80386 LSS instruction, and the problem will not occur.
*/


#include <asm/segment.h>     // for __KERNEL_CS_SELECTOR

.macro ISR_NOERRCODE arg1  # define a macro, taking one parameter
	.globl isr\arg1        # %1 accesses the first parameter.
  isr\arg1:
    pushl $0                    # Push a dummy error code.
    pushl $\arg1                # Push the interrupt number.
    jmp isr_common_stub         # Go to our common handler code.
.endm

# This macro creates a stub for an ISR which passes it's own error code.
.macro ISR_ERRCODE arg1
  .globl isr\arg1
  isr\arg1:
    pushl $\arg1                # Push the interrupt number.
    jmp isr_common_stub
.endm

# This macro creates a stub for an IRQ - the first parameter is
# the IRQ number, the second is the ISR number it is remapped to.
.macro IRQ arg1 arg2
	.globl irq\arg1
  irq\arg1:
    pushl $0        # always push a dummy err code so that we can reuse the trapframe struct
    pushl $\arg2
    jmp isr_common_stub
.endm

ISR_NOERRCODE 0
ISR_NOERRCODE 1
ISR_NOERRCODE 2
ISR_NOERRCODE 3
ISR_NOERRCODE 4
ISR_NOERRCODE 5
ISR_NOERRCODE 6
ISR_NOERRCODE 7
ISR_ERRCODE   8
ISR_NOERRCODE 9
ISR_ERRCODE   10
ISR_ERRCODE   11
ISR_ERRCODE   12
ISR_ERRCODE   13
ISR_ERRCODE   14
ISR_NOERRCODE 15
ISR_NOERRCODE 16
ISR_NOERRCODE 17
ISR_NOERRCODE 18
ISR_NOERRCODE 19
ISR_NOERRCODE 20
ISR_NOERRCODE 21
ISR_NOERRCODE 22
ISR_NOERRCODE 23
ISR_NOERRCODE 24
ISR_NOERRCODE 25
ISR_NOERRCODE 26
ISR_NOERRCODE 27
ISR_NOERRCODE 28
ISR_NOERRCODE 29
ISR_NOERRCODE 30
ISR_NOERRCODE 31

#ISR_NOERRCODE 0x80

IRQ   0,    32
IRQ   1,    33
IRQ   2,    34
IRQ   3,    35
IRQ   4,    36
IRQ   5,    37
IRQ   6,    38
IRQ   7,    39
IRQ   8,    40
IRQ   9,    41
IRQ  10,    42
IRQ  11,    43
IRQ  12,    44
IRQ  13,    45
IRQ  14,    46
IRQ  15,    47

.extern trap_handler

# This is our common ISR stub. It saves the processor state, sets
# up for kernel mode segments, calls the C-level fault handler,
# and finally restores the stack frame.
isr_common_stub:
    // build trap frame
    pushl %ds
    pushl %es
    pushl %fs
    pushl %gs
    pushal                    # Pushes edi,esi,ebp,esp,ebx,edx,ecx,eax (see struct trapframe)

  # Set up data segments.
    movw $__KERNEL_DS_SELECTOR, %ax  # load the kernel data segment descriptor
    movw %ax, %ds
    movw %ax, %es
// TODO fs y gs??

  # Call trap_handler(tf), where tf=%esp
    pushl %esp
    call trap_handler
    addl $4, %esp

    # Return falls through to trapret...
.globl trapret
trapret:
    popal                     # Pops edi,esi,ebp...
    popl %gs
    popl %fs
    popl %es
    popl %ds
    addl $0x8, %esp     # Cleans up the pushed error code and pushed ISR number
    iret           # pops 5 things at once: CS, EIP, EFLAGS, SS, and ESP

.altmacro

.macro IRQ_VECTOR arg1
	.long irq\arg1
.endm

.section .rodata
.globl irq_vectors
irq_vectors:
.set i,0
.rept 16
    IRQ_VECTOR %i
    .set i, i+1
.endr
