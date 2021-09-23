//
// isr.c -- High level interrupt service routines and interrupt request handlers.
// Part of this code is modified from Bran's kernel development tutorials.
// Rewritten for JamesM's kernel development tutorials.
//

#include <asm/traps.h>
#include <asm/segment.h>     // for __KERNEL_CS_SELECTOR
#include <asm/irq.h>

#include <brunix/console.h>
#include <brunix/kernel.h>




extern void init_idt(void);
extern void irq_init(void);
void traps_init(void);
void set_intr_gate(uint8_t n, uint32_t addr);
void set_trap_gate(uint8_t n, uint32_t addr);

/*PRIVATE*/ void dividebyzero(__attribute__((unused)) struct trapframe *regs);

extern void pic_acknowledge(uint32_t int_no); //TODO revisar

#define MAX_HANDLERS 256
#include <stddef.h>     // for NULL
#include <stdbool.h>
#pragma GCC diagnostic ignored "-Wpedantic"
static trap_handler_t trap_handlers[MAX_HANDLERS] = {[0 ... MAX_HANDLERS-1] = NULL };


void register_interrupt_handler(uint8_t n, trap_handler_t handler) {
    printk("Registering interrupt handler number %d, handler %x!\n", n, handler);
    trap_handlers[n] = handler;
}

bool is_interrupt_handler_registered(uint8_t n) {
    return trap_handlers[n] != NULL;
}

// This gets called from our ASM interrupt handler stub.
void trap_handler(struct trapframe *tf) {
    if (tf->trap_no != 32)
        printk("Calling trap_handler() for trap number 0x%x! (code %d eip %p)\n", tf->trap_no, tf->err_code, tf->eip);

//    if(tf->trapno == T_SYSCALL){
//    }

    switch (tf->trap_no) {
        case T_IRQ0 + IRQ_TIMER:
//            wakeup(&ticks);     //TODO despertar a procesos esperando en queue "timer"
            break;
        default:
            printk("DEFAULT ISR\n");
    }
    if (trap_handlers[tf->trap_no] != 0) {
        if (tf->trap_no >= 32 && tf->trap_no <= 40) {
            /* Send an EOI (end of interrupt) signal to the PICs. */
            pic_acknowledge(tf->trap_no);
        }
        trap_handler_t handler = trap_handlers[tf->trap_no];
        handler(tf);
    }
    else {
        printk("*** Unhandled interrupt: 0x%x (error code: %d)\n", tf->trap_no, tf->err_code);
        printk("CS: 0x%x; EIP: 0x%x; EFLAGS = %b", tf->cs, tf->eip, tf->eflags);
    }
}


/*PRIVATE*/ void dividebyzero(__attribute__((unused)) struct trapframe *regs) {
    printk("Processor exception: divide by zero!\n");
}


void isr_install(void) {

    /*
     * From 80386 manual: "The NMI and the exceptions recognized by the processor are assigned
predetermined identifiers in the range 0 through 31"
     */
    set_trap_gate(0, (uint32_t) isr0);

//    set_intr_gate(X86_TRAP_DE, &divide_error);
//    set_intr_gate_ist(X86_TRAP_NMI, &nmi, NMI_STACK);
//    /* int4 can be called from all */
//    set_system_intr_gate(X86_TRAP_OF, &overflow);
//    set_intr_gate(X86_TRAP_BR, &bounds);
//    set_intr_gate(X86_TRAP_UD, &invalid_op);
//    set_intr_gate(X86_TRAP_NM, &device_not_available);
//    set_task_gate(X86_TRAP_DF, GDT_ENTRY_DOUBLEFAULT_TSS);
//    set_intr_gate(X86_TRAP_OLD_MF, &coprocessor_segment_overrun);
//    set_intr_gate(X86_TRAP_TS, &invalid_TSS);
//    set_intr_gate(X86_TRAP_NP, &segment_not_present);
//    set_intr_gate(X86_TRAP_SS, stack_segment);
//    set_intr_gate(X86_TRAP_GP, &general_protection);
//    set_intr_gate(X86_TRAP_SPURIOUS, &spurious_interrupt_bug);
//    set_intr_gate(X86_TRAP_MF, &coprocessor_error);
//    set_intr_gate(X86_TRAP_AC, &alignment_check);
//    set_intr_gate(X86_TRAP_XF, &simd_coprocessor_error);
//
//    set_system_trap_gate(SYSCALL_VECTOR, &system_call);
//
//    for (i = FIRST_EXTERNAL_VECTOR; i < NR_VECTORS; i++) {
//        /* IA32_SYSCALL_VECTOR could be used in trap_init already. */
//        if (!test_bit(i, used_vectors))
//            set_intr_gate(i, interrupt[i-FIRST_EXTERNAL_VECTOR]);
//    }

    /* exceptions */
//    set_intr_gate(32, isr0);

//    idt_set_gate( 0, (uint32_t)isr0 , __KERNEL_CS_SELECTOR, IDT_FLAG_PRESENT|IDT_FLAG_RING0|IDT_FLAG_32BIT|IDT_FLAG_INTTRAP);
//    idt_set_gate( 1, (uint32_t)isr1 , __KERNEL_CS_SELECTOR, IDT_FLAG_PRESENT|IDT_FLAG_RING0|IDT_FLAG_32BIT|IDT_FLAG_INTTRAP);
//    idt_set_gate( 2, (uint32_t)isr2 , __KERNEL_CS_SELECTOR, IDT_FLAG_PRESENT|IDT_FLAG_RING0|IDT_FLAG_32BIT|IDT_FLAG_INTTRAP);
//    idt_set_gate( 3, (uint32_t)isr3 , __KERNEL_CS_SELECTOR, IDT_FLAG_PRESENT|IDT_FLAG_RING0|IDT_FLAG_32BIT|IDT_FLAG_INTTRAP);
//    idt_set_gate( 4, (uint32_t)isr4 , __KERNEL_CS_SELECTOR, IDT_FLAG_PRESENT|IDT_FLAG_RING0|IDT_FLAG_32BIT|IDT_FLAG_INTTRAP);
//    idt_set_gate( 5, (uint32_t)isr5 , __KERNEL_CS_SELECTOR, IDT_FLAG_PRESENT|IDT_FLAG_RING0|IDT_FLAG_32BIT|IDT_FLAG_INTTRAP);
//    idt_set_gate( 6, (uint32_t)isr6 , __KERNEL_CS_SELECTOR, IDT_FLAG_PRESENT|IDT_FLAG_RING0|IDT_FLAG_32BIT|IDT_FLAG_INTTRAP);
//    idt_set_gate( 7, (uint32_t)isr7 , __KERNEL_CS_SELECTOR, IDT_FLAG_PRESENT|IDT_FLAG_RING0|IDT_FLAG_32BIT|IDT_FLAG_INTTRAP);
//    idt_set_gate( 8, (uint32_t)isr8 , __KERNEL_CS_SELECTOR, IDT_FLAG_PRESENT|IDT_FLAG_RING0|IDT_FLAG_32BIT|IDT_FLAG_INTTRAP);
//    idt_set_gate( 9, (uint32_t)isr9 , __KERNEL_CS_SELECTOR, IDT_FLAG_PRESENT|IDT_FLAG_RING0|IDT_FLAG_32BIT|IDT_FLAG_INTTRAP);
//    idt_set_gate(10, (uint32_t)isr10, __KERNEL_CS_SELECTOR, IDT_FLAG_PRESENT|IDT_FLAG_RING0|IDT_FLAG_32BIT|IDT_FLAG_INTTRAP);
//    idt_set_gate(11, (uint32_t)isr11, __KERNEL_CS_SELECTOR, IDT_FLAG_PRESENT|IDT_FLAG_RING0|IDT_FLAG_32BIT|IDT_FLAG_INTTRAP);
//    idt_set_gate(12, (uint32_t)isr12, __KERNEL_CS_SELECTOR, IDT_FLAG_PRESENT|IDT_FLAG_RING0|IDT_FLAG_32BIT|IDT_FLAG_INTTRAP);
//    idt_set_gate(13, (uint32_t)isr13, __KERNEL_CS_SELECTOR, IDT_FLAG_PRESENT|IDT_FLAG_RING0|IDT_FLAG_32BIT|IDT_FLAG_INTTRAP);
//    idt_set_gate(14, (uint32_t)isr14, __KERNEL_CS_SELECTOR, IDT_FLAG_PRESENT|IDT_FLAG_RING0|IDT_FLAG_32BIT|IDT_FLAG_INTTRAP);
//    idt_set_gate(15, (uint32_t)isr15, __KERNEL_CS_SELECTOR, IDT_FLAG_PRESENT|IDT_FLAG_RING0|IDT_FLAG_32BIT|IDT_FLAG_INTTRAP);
//    idt_set_gate(16, (uint32_t)isr16, __KERNEL_CS_SELECTOR, IDT_FLAG_PRESENT|IDT_FLAG_RING0|IDT_FLAG_32BIT|IDT_FLAG_INTTRAP);
//    idt_set_gate(17, (uint32_t)isr17, __KERNEL_CS_SELECTOR, IDT_FLAG_PRESENT|IDT_FLAG_RING0|IDT_FLAG_32BIT|IDT_FLAG_INTTRAP);
//    idt_set_gate(18, (uint32_t)isr18, __KERNEL_CS_SELECTOR, IDT_FLAG_PRESENT|IDT_FLAG_RING0|IDT_FLAG_32BIT|IDT_FLAG_INTTRAP);
//    idt_set_gate(19, (uint32_t)isr19, __KERNEL_CS_SELECTOR, IDT_FLAG_PRESENT|IDT_FLAG_RING0|IDT_FLAG_32BIT|IDT_FLAG_INTTRAP);
//    idt_set_gate(20, (uint32_t)isr20, __KERNEL_CS_SELECTOR, IDT_FLAG_PRESENT|IDT_FLAG_RING0|IDT_FLAG_32BIT|IDT_FLAG_INTTRAP);
//    idt_set_gate(21, (uint32_t)isr21, __KERNEL_CS_SELECTOR, IDT_FLAG_PRESENT|IDT_FLAG_RING0|IDT_FLAG_32BIT|IDT_FLAG_INTTRAP);
//    idt_set_gate(22, (uint32_t)isr22, __KERNEL_CS_SELECTOR, IDT_FLAG_PRESENT|IDT_FLAG_RING0|IDT_FLAG_32BIT|IDT_FLAG_INTTRAP);
//    idt_set_gate(23, (uint32_t)isr23, __KERNEL_CS_SELECTOR, IDT_FLAG_PRESENT|IDT_FLAG_RING0|IDT_FLAG_32BIT|IDT_FLAG_INTTRAP);
//    idt_set_gate(24, (uint32_t)isr24, __KERNEL_CS_SELECTOR, IDT_FLAG_PRESENT|IDT_FLAG_RING0|IDT_FLAG_32BIT|IDT_FLAG_INTTRAP);
//    idt_set_gate(25, (uint32_t)isr25, __KERNEL_CS_SELECTOR, IDT_FLAG_PRESENT|IDT_FLAG_RING0|IDT_FLAG_32BIT|IDT_FLAG_INTTRAP);
//    idt_set_gate(26, (uint32_t)isr26, __KERNEL_CS_SELECTOR, IDT_FLAG_PRESENT|IDT_FLAG_RING0|IDT_FLAG_32BIT|IDT_FLAG_INTTRAP);
//    idt_set_gate(27, (uint32_t)isr27, __KERNEL_CS_SELECTOR, IDT_FLAG_PRESENT|IDT_FLAG_RING0|IDT_FLAG_32BIT|IDT_FLAG_INTTRAP);
//    idt_set_gate(28, (uint32_t)isr28, __KERNEL_CS_SELECTOR, IDT_FLAG_PRESENT|IDT_FLAG_RING0|IDT_FLAG_32BIT|IDT_FLAG_INTTRAP);
//    idt_set_gate(29, (uint32_t)isr29, __KERNEL_CS_SELECTOR, IDT_FLAG_PRESENT|IDT_FLAG_RING0|IDT_FLAG_32BIT|IDT_FLAG_INTTRAP);
//    idt_set_gate(30, (uint32_t)isr30, __KERNEL_CS_SELECTOR, IDT_FLAG_PRESENT|IDT_FLAG_RING0|IDT_FLAG_32BIT|IDT_FLAG_INTTRAP);
//    idt_set_gate(31, (uint32_t)isr31, __KERNEL_CS_SELECTOR, IDT_FLAG_PRESENT|IDT_FLAG_RING0|IDT_FLAG_32BIT|IDT_FLAG_INTTRAP);

    /* syscalls */
//    idt_set_gate(0x80, (uint32_t)isr0x80, 0x08, 0x8E);

for (uint8_t i=0;i<1;i++)
    register_interrupt_handler(i, &dividebyzero);
}

void traps_init(void) {
    irq_init();
    init_idt();
    isr_install();
}