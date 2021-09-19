#include <asm/irq.h>
#include <asm/traps.h>
#include <asm/io.h>
#include <asm/segment.h>     // for __KERNEL_CS_SELECTOR
#include <brunix/console.h>
#include <brunix/kernel.h>
#include <brunix/errno.h>
#include <stddef.h>

void pic_acknowledge(uint32_t int_no);
void irq_handler(struct trapframe *regs);
PRIVATE void pic_init(void);
void irq_init(void);
extern void set_intr_gate(unsigned int n, uint32_t addr);

extern isr_t trap_handlers[MAX_HANDLERS];


int request_irq(uint8_t irq_nr, isr_t handler) {
    if (irq_nr >= IRQS_COUNT || !handler)
        return -EINVAL;

    /*
     * The range 0 through 31 is reserved, so we offset IRQs by 32 (see arch/x86/kernel/traps.c).
     */

    if (trap_handlers[irq_nr + 32])
        return -EBUSY;

    printk("Registering IRQ%d handler...\n", irq_nr);

    trap_handlers[irq_nr + 32] = handler;

    set_intr_gate((unsigned int)(irq_nr+32), ((uint32_t) irq0) + ((uint32_t) (6 * (irq_nr))));   // 6 = size of any irqXXX binary block

    return 0;
}

// http://wiki.osdev.org/PIC
static void pic_remap(uint8_t offset1, uint8_t offset2) {
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

/**
 * From 80386 manual: "The identifiers of the maskable interrupts are determined by external
interrupt controllers (such as Intel's 8259A Programmable Interrupt
Controller) and communicated to the processor during the processor's
interrupt-acknowledge sequence. The numbers assigned by an 8259A PIC can be
specified by software. Any numbers in the range 32 through 255 can be used."
 */
PRIVATE void pic_init(void) {
    // Remap the irq table.
    //The PICs are communicated with via the I/O bus. Each has a command port and a data port:
    //Master - command: 0x20, data: 0x21
    //Slave - command: 0xA0, data: 0xA1
    printk("Remapping PIC...");
    pic_remap(0x20, 0x28);
}

void pic_acknowledge(uint32_t int_no) {
    if (int_no >= 8) {
        //printk("Resetting slave!", 0);
        outb (PIC2_COMMAND, PIC_EOI); /* Send reset signal to slave */
    }
    //printk("Resetting master!");
    outb (PIC1_COMMAND, PIC_EOI); /* Send reset signal to master. (As well as slave, if necessary). */


    outb(0x61, inb(0x61) | 0x03); //speaker
}

void irq_init(void) {
    pic_init();
}