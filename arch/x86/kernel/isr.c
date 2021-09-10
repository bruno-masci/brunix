//
// isr.c -- High level interrupt service routines and interrupt request handlers.
// Part of this code is modified from Bran's kernel development tutorials.
// Rewritten for JamesM's kernel development tutorials.
//

#include "idt.h"

#include <asm/isr.h>
#include <asm/segment.h>     // for __KERNEL_CS_SELECTOR.

#include <brunix/console.h>
#include <brunix/kernel.h>


/*PRIVATE*/ void dividebyzero(__attribute__((unused)) struct registers_t *regs);

extern void pic_acknowledge(uint32_t int_no); //TODO revisar

#define MAX_HANDLERS 256
#include <stddef.h>
#pragma GCC diagnostic ignored "-Wpedantic"
/*static */isr_t interrupt_handlers[MAX_HANDLERS] = {[0 ... MAX_HANDLERS-1] = NULL };


void register_interrupt_handler(uint8_t n, isr_t handler) {
    printk("Registering interrupt handler number %d, handler %x!\n", n, handler);
    interrupt_handlers[n] = handler;
}

// This gets called from our ASM interrupt handler stub.
void isr_handler(struct registers_t *regs) {
    /* This line is important. When the processor extends the 8-bit interrupt number
     * to a 32bit value, it sign-extends, not zero extends. So if the most significant
     * bit (0x80) is set, regs.int_no will be very large (about 0xFFFFFF80).
     */
    uint8_t int_no = regs->int_no & 0xFF;
    printk("Calling isr_handler() for INT number (code %d eip %p) 0x%x!\n", regs->err_code, regs->eip, int_no);

    if (interrupt_handlers[int_no] != 0) {
        printk("entre\n\n");
        if (regs->int_no >= 32 && regs->int_no <= 40) {
            pic_acknowledge(regs->int_no);
        }
        isr_t handler = interrupt_handlers[int_no];
        handler(regs);
    }
    else {
        printk("*** Unhandled interrupt: 0x%x (error code: %d)\n", regs->int_no, regs->err_code);
        printk("CS: 0x%x; EIP: 0x%x; EFLAGS = %b", regs->cs, regs->eip, regs->eflags);
    }
}
#include <asm/io.h>
/*PRIVATE*/ void dividebyzero(__attribute__((unused)) struct registers_t *regs) {
    printk("Processor exception: divide by zero!\n");
//    asm volatile("sti");

    if (regs->int_no!=16)
        outb(0x20, 0x20);
}


void isr_install(void) {
    /* exceptions */
    idt_set_gate( 0, (uint32_t)isr0 , __KERNEL_CS_SELECTOR, IDT_FLAG_PRESENT|IDT_FLAG_RING0|IDT_FLAG_32BIT|IDT_FLAG_INTTRAP);
    idt_set_gate( 1, (uint32_t)isr1 , __KERNEL_CS_SELECTOR, IDT_FLAG_PRESENT|IDT_FLAG_RING0|IDT_FLAG_32BIT|IDT_FLAG_INTTRAP);
    idt_set_gate( 2, (uint32_t)isr2 , __KERNEL_CS_SELECTOR, IDT_FLAG_PRESENT|IDT_FLAG_RING0|IDT_FLAG_32BIT|IDT_FLAG_INTTRAP);
    idt_set_gate( 3, (uint32_t)isr3 , __KERNEL_CS_SELECTOR, IDT_FLAG_PRESENT|IDT_FLAG_RING0|IDT_FLAG_32BIT|IDT_FLAG_INTTRAP);
    idt_set_gate( 4, (uint32_t)isr4 , __KERNEL_CS_SELECTOR, IDT_FLAG_PRESENT|IDT_FLAG_RING0|IDT_FLAG_32BIT|IDT_FLAG_INTTRAP);
    idt_set_gate( 5, (uint32_t)isr5 , __KERNEL_CS_SELECTOR, IDT_FLAG_PRESENT|IDT_FLAG_RING0|IDT_FLAG_32BIT|IDT_FLAG_INTTRAP);
    idt_set_gate( 6, (uint32_t)isr6 , __KERNEL_CS_SELECTOR, IDT_FLAG_PRESENT|IDT_FLAG_RING0|IDT_FLAG_32BIT|IDT_FLAG_INTTRAP);
    idt_set_gate( 7, (uint32_t)isr7 , __KERNEL_CS_SELECTOR, IDT_FLAG_PRESENT|IDT_FLAG_RING0|IDT_FLAG_32BIT|IDT_FLAG_INTTRAP);
    idt_set_gate( 8, (uint32_t)isr8 , __KERNEL_CS_SELECTOR, IDT_FLAG_PRESENT|IDT_FLAG_RING0|IDT_FLAG_32BIT|IDT_FLAG_INTTRAP);
    idt_set_gate( 9, (uint32_t)isr9 , __KERNEL_CS_SELECTOR, IDT_FLAG_PRESENT|IDT_FLAG_RING0|IDT_FLAG_32BIT|IDT_FLAG_INTTRAP);
    idt_set_gate(10, (uint32_t)isr10, __KERNEL_CS_SELECTOR, IDT_FLAG_PRESENT|IDT_FLAG_RING0|IDT_FLAG_32BIT|IDT_FLAG_INTTRAP);
    idt_set_gate(11, (uint32_t)isr11, __KERNEL_CS_SELECTOR, IDT_FLAG_PRESENT|IDT_FLAG_RING0|IDT_FLAG_32BIT|IDT_FLAG_INTTRAP);
    idt_set_gate(12, (uint32_t)isr12, __KERNEL_CS_SELECTOR, IDT_FLAG_PRESENT|IDT_FLAG_RING0|IDT_FLAG_32BIT|IDT_FLAG_INTTRAP);
    idt_set_gate(13, (uint32_t)isr13, __KERNEL_CS_SELECTOR, IDT_FLAG_PRESENT|IDT_FLAG_RING0|IDT_FLAG_32BIT|IDT_FLAG_INTTRAP);
    idt_set_gate(14, (uint32_t)isr14, __KERNEL_CS_SELECTOR, IDT_FLAG_PRESENT|IDT_FLAG_RING0|IDT_FLAG_32BIT|IDT_FLAG_INTTRAP);
    idt_set_gate(15, (uint32_t)isr15, __KERNEL_CS_SELECTOR, IDT_FLAG_PRESENT|IDT_FLAG_RING0|IDT_FLAG_32BIT|IDT_FLAG_INTTRAP);
    idt_set_gate(16, (uint32_t)isr16, __KERNEL_CS_SELECTOR, IDT_FLAG_PRESENT|IDT_FLAG_RING0|IDT_FLAG_32BIT|IDT_FLAG_INTTRAP);
    idt_set_gate(17, (uint32_t)isr17, __KERNEL_CS_SELECTOR, IDT_FLAG_PRESENT|IDT_FLAG_RING0|IDT_FLAG_32BIT|IDT_FLAG_INTTRAP);
    idt_set_gate(18, (uint32_t)isr18, __KERNEL_CS_SELECTOR, IDT_FLAG_PRESENT|IDT_FLAG_RING0|IDT_FLAG_32BIT|IDT_FLAG_INTTRAP);
    idt_set_gate(19, (uint32_t)isr19, __KERNEL_CS_SELECTOR, IDT_FLAG_PRESENT|IDT_FLAG_RING0|IDT_FLAG_32BIT|IDT_FLAG_INTTRAP);
    idt_set_gate(20, (uint32_t)isr20, __KERNEL_CS_SELECTOR, IDT_FLAG_PRESENT|IDT_FLAG_RING0|IDT_FLAG_32BIT|IDT_FLAG_INTTRAP);
    idt_set_gate(21, (uint32_t)isr21, __KERNEL_CS_SELECTOR, IDT_FLAG_PRESENT|IDT_FLAG_RING0|IDT_FLAG_32BIT|IDT_FLAG_INTTRAP);
    idt_set_gate(22, (uint32_t)isr22, __KERNEL_CS_SELECTOR, IDT_FLAG_PRESENT|IDT_FLAG_RING0|IDT_FLAG_32BIT|IDT_FLAG_INTTRAP);
    idt_set_gate(23, (uint32_t)isr23, __KERNEL_CS_SELECTOR, IDT_FLAG_PRESENT|IDT_FLAG_RING0|IDT_FLAG_32BIT|IDT_FLAG_INTTRAP);
    idt_set_gate(24, (uint32_t)isr24, __KERNEL_CS_SELECTOR, IDT_FLAG_PRESENT|IDT_FLAG_RING0|IDT_FLAG_32BIT|IDT_FLAG_INTTRAP);
    idt_set_gate(25, (uint32_t)isr25, __KERNEL_CS_SELECTOR, IDT_FLAG_PRESENT|IDT_FLAG_RING0|IDT_FLAG_32BIT|IDT_FLAG_INTTRAP);
    idt_set_gate(26, (uint32_t)isr26, __KERNEL_CS_SELECTOR, IDT_FLAG_PRESENT|IDT_FLAG_RING0|IDT_FLAG_32BIT|IDT_FLAG_INTTRAP);
    idt_set_gate(27, (uint32_t)isr27, __KERNEL_CS_SELECTOR, IDT_FLAG_PRESENT|IDT_FLAG_RING0|IDT_FLAG_32BIT|IDT_FLAG_INTTRAP);
    idt_set_gate(28, (uint32_t)isr28, __KERNEL_CS_SELECTOR, IDT_FLAG_PRESENT|IDT_FLAG_RING0|IDT_FLAG_32BIT|IDT_FLAG_INTTRAP);
    idt_set_gate(29, (uint32_t)isr29, __KERNEL_CS_SELECTOR, IDT_FLAG_PRESENT|IDT_FLAG_RING0|IDT_FLAG_32BIT|IDT_FLAG_INTTRAP);
    idt_set_gate(30, (uint32_t)isr30, __KERNEL_CS_SELECTOR, IDT_FLAG_PRESENT|IDT_FLAG_RING0|IDT_FLAG_32BIT|IDT_FLAG_INTTRAP);
    idt_set_gate(31, (uint32_t)isr31, __KERNEL_CS_SELECTOR, IDT_FLAG_PRESENT|IDT_FLAG_RING0|IDT_FLAG_32BIT|IDT_FLAG_INTTRAP);

    /* syscalls */
//    idt_set_gate(0x80, (uint32_t)isr0x80, 0x08, 0x8E);

for (int i=0;i<32;i++)
    register_interrupt_handler(i, &dividebyzero);
}
