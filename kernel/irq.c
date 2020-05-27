#include <arch/x86/irq.h>
#include <arch/x86/idt.h>
#include <arch/x86/isr.h>
#include <arch/x86/io.h>
#include <arch/x86/asm.h>     // for __KERNEL_CS_SELECTOR. FIXME mover a mmu o un lugar mejor
#include <brunix/console.h>
#include <stddef.h>


/** @brief IRQ handle pointers
 *
 * This array is actually an array of function pointers. We use
 * this to handle custom IRQ handlers for a given IRQ
 */
//#pragma GCC diagnostic ignored "-Wpedantic"
//static void* irq_routines[MAX_HANDLERS] = {[0 ... MAX_HANDLERS-1] = NULL };


#pragma GCC diagnostic ignored "-Wpedantic"
//static isr_t irq_handlers[256];//TODO revisar = {[0 ... 255] = {0, 0, 0, 0, 0, 0}};




void register_irq_handler(uint8_t n, isr_t handler) {
    debug("Registering IRQ handler number %d (%d), handler %x!...", n - 32, n, handler);
    register_interrupt_handler(n, handler);
}

// http://wiki.osdev.org/PIC
static void pic_remap(int offset1, int offset2) {
    uint8_t a1, a2;

    a1 = inb(PIC1_DATA);			// save masks
    a2 = inb(PIC2_DATA);

    outb(PIC1_COMMAND, 0x11);		// starts the initialization sequence (in cascade mode)
    outb(PIC2_COMMAND, 0x11);
    outb(PIC1_DATA, offset1);		// ICW2: Master PIC vector offset
    outb(PIC2_DATA, offset2);		// ICW2: Slave PIC vector offset
    outb(PIC1_DATA, 0x04);			// ICW3: tell Master PIC that there is a slave PIC at IRQ2 (0000 0100)
    outb(PIC2_DATA, 0x02);			// ICW3: tell Slave PIC its cascade identity (0000 0010)

    outb(PIC1_DATA, 0x01);
    outb(PIC2_DATA, 0x01);

    outb(PIC1_DATA, a1);			// restore saved masks.
    outb(PIC2_DATA, a2);
}


void irq_install(void) {
    // Remap the irq table.
    //The PICs are communicated with via the I/O bus. Each has a command port and a data port:
    //Master - command: 0x20, data: 0x21
    //Slave - command: 0xA0, data: 0xA1
    debug_noargs("Remapping PIC...");
    pic_remap(0x20, 0x28);

    idt_set_gate(IRQ0, (uint32_t) irq0, __KERNEL_CS_SELECTOR, 0);
    idt_set_gate(IRQ1, (uint32_t) irq1, __KERNEL_CS_SELECTOR, 0);
    idt_set_gate(IRQ2, (uint32_t) irq2, __KERNEL_CS_SELECTOR, 0);
    idt_set_gate(IRQ3, (uint32_t) irq3, __KERNEL_CS_SELECTOR, 0);
    idt_set_gate(IRQ4, (uint32_t) irq4, __KERNEL_CS_SELECTOR, 0);
    idt_set_gate(IRQ5, (uint32_t) irq5, __KERNEL_CS_SELECTOR, 0);
    idt_set_gate(IRQ6, (uint32_t) irq6, __KERNEL_CS_SELECTOR, 0);
    idt_set_gate(IRQ7, (uint32_t) irq7, __KERNEL_CS_SELECTOR, 0);
    idt_set_gate(IRQ8, (uint32_t) irq8, __KERNEL_CS_SELECTOR, 0);
    idt_set_gate(IRQ9, (uint32_t) irq9, __KERNEL_CS_SELECTOR, 0);
    idt_set_gate(IRQ10, (uint32_t) irq10, __KERNEL_CS_SELECTOR, 0);
    idt_set_gate(IRQ11, (uint32_t) irq11, __KERNEL_CS_SELECTOR, 0);
    idt_set_gate(IRQ12, (uint32_t) irq12, __KERNEL_CS_SELECTOR, 0);
    idt_set_gate(IRQ13, (uint32_t) irq13, __KERNEL_CS_SELECTOR, 0);
    idt_set_gate(IRQ14, (uint32_t) irq14, __KERNEL_CS_SELECTOR, 0);
    idt_set_gate(IRQ15, (uint32_t) irq15, __KERNEL_CS_SELECTOR, 0);
}

void pic_acknowledge(uint32_t int_no) {
//    cprintf("DDDDDDDDDDDDDd");
    if (int_no >= 40) {
        //debug("Resetting slave!", 0);
        outb (PIC2_COMMAND, PIC_EOI); /* Send reset signal to slave */
    }
    //debug("Resetting master!");
    outb (PIC1_COMMAND, PIC_EOI); /* Send reset signal to master. (As well as slave, if necessary). */
}

void irq_init(void) {
    irq_install();
    isr_install();
    init_idt();
}