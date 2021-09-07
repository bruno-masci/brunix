#include <asm/irq.h>
#include <asm/idt.h>
#include <asm/isr.h>
#include <asm/io.h>
#include <asm/segment.h>     // for __KERNEL_CS_SELECTOR.
#include <brunix/console.h>
#include <brunix/kernel.h>
#include <brunix/errno.h>
#include <stddef.h>


/** @brief IRQ handle pointers
 *
 * This array is actually an array of function pointers. We use
 * this to handle custom IRQ handlers for a given IRQ
 */
//#pragma GCC diagnostic ignored "-Wpedantic"
static isr_t irq_handlers[IRQS_COUNT] = {[0 ... IRQS_COUNT-1] = NULL };



void irq_install(void);
void pic_acknowledge(uint32_t int_no);
void irq_handler(struct registers_t *regs);
void pic_init(void);


int request_irq(uint8_t irq_nr, isr_t handler) {
    if (irq_nr < 0 || irq_nr >= IRQS_COUNT)
        return -EINVAL;

    if (irq_handlers[irq_nr])
        return -EBUSY;

    if (!handler)
        return -EINVAL;

    printk("Registering IRQ%d handler...\n", irq_nr);
//    save_flags(flags);
//    cli();
    irq_handlers[irq_nr] = handler;

    if (irq_nr==0) {
        /// This bit shall be set to 0 if the IDT slot is empty
        #define IDT_FLAG_PRESENT 	0x80
        /// Interrupt can be called from within RING0
        #define IDT_FLAG_RING0		0x00
        /// Interrupt can be called from within RING1 and lower
        #define IDT_FLAG_RING1		0x20
        /// Interrupt can be called from within RING2 and lower
        #define IDT_FLAG_RING2		0x40
        /// Interrupt can be called from within RING3 and lower
        #define IDT_FLAG_RING3		0x60
        /// Size of gate is 16 bit
        #define IDT_FLAG_16BIT		0x00
        /// Size of gate is 32 bit
        #define IDT_FLAG_32BIT		0x08
        /// The entry describes an interrupt gate
        #define IDT_FLAG_INTTRAP	0x06
        /// The entry describes a trap gate
        #define IDT_FLAG_TRAPGATE	0x07
        /// The entry describes a task gate
        #define IDT_FLAG_TASKGATE	0x05
        idt_set_gate(IRQ0, (uint32_t) irq0, __KERNEL_CS_SELECTOR, IDT_FLAG_PRESENT|IDT_FLAG_RING0|IDT_FLAG_32BIT|IDT_FLAG_INTTRAP);
    }
//    set_intr_gate(0x20+irq,fast_interrupt[irq]);
//    restore_flags(flags);
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

void pic_init(void) {
    // Remap the irq table.
    //The PICs are communicated with via the I/O bus. Each has a command port and a data port:
    //Master - command: 0x20, data: 0x21
    //Slave - command: 0xA0, data: 0xA1
    printk("Remapping PIC...");
    pic_remap(0x20, 0x28);
}


void irq_install(void) {
        /// This bit shall be set to 0 if the IDT slot is empty
#define IDT_FLAG_PRESENT 	0x80
/// Interrupt can be called from within RING0
#define IDT_FLAG_RING0		0x00
/// Interrupt can be called from within RING1 and lower
#define IDT_FLAG_RING1		0x20
/// Interrupt can be called from within RING2 and lower
#define IDT_FLAG_RING2		0x40
/// Interrupt can be called from within RING3 and lower
#define IDT_FLAG_RING3		0x60
/// Size of gate is 16 bit
#define IDT_FLAG_16BIT		0x00
/// Size of gate is 32 bit
#define IDT_FLAG_32BIT		0x08
/// The entry describes an interrupt gate
#define IDT_FLAG_INTTRAP	0x06
/// The entry describes a trap gate
#define IDT_FLAG_TRAPGATE	0x07
/// The entry describes a task gate
#define IDT_FLAG_TASKGATE	0x05
//    idt_set_gate(IRQ0, (uint32_t) irq0, __KERNEL_CS_SELECTOR, IDT_FLAG_PRESENT|IDT_FLAG_RING0|IDT_FLAG_32BIT|IDT_FLAG_INTTRAP);
    idt_set_gate(IRQ1, (uint32_t) irq1, __KERNEL_CS_SELECTOR, IDT_FLAG_PRESENT|IDT_FLAG_RING0|IDT_FLAG_32BIT|IDT_FLAG_INTTRAP);
    idt_set_gate(IRQ2, (uint32_t) irq2, __KERNEL_CS_SELECTOR, IDT_FLAG_PRESENT|IDT_FLAG_RING0|IDT_FLAG_32BIT|IDT_FLAG_INTTRAP);
    idt_set_gate(IRQ3, (uint32_t) irq3, __KERNEL_CS_SELECTOR, IDT_FLAG_PRESENT|IDT_FLAG_RING0|IDT_FLAG_32BIT|IDT_FLAG_INTTRAP);
    idt_set_gate(IRQ4, (uint32_t) irq4, __KERNEL_CS_SELECTOR, IDT_FLAG_PRESENT|IDT_FLAG_RING0|IDT_FLAG_32BIT|IDT_FLAG_INTTRAP);
    idt_set_gate(IRQ5, (uint32_t) irq5, __KERNEL_CS_SELECTOR, IDT_FLAG_PRESENT|IDT_FLAG_RING0|IDT_FLAG_32BIT|IDT_FLAG_INTTRAP);
    idt_set_gate(IRQ6, (uint32_t) irq6, __KERNEL_CS_SELECTOR, IDT_FLAG_PRESENT|IDT_FLAG_RING0|IDT_FLAG_32BIT|IDT_FLAG_INTTRAP);
    idt_set_gate(IRQ7, (uint32_t) irq7, __KERNEL_CS_SELECTOR, IDT_FLAG_PRESENT|IDT_FLAG_RING0|IDT_FLAG_32BIT|IDT_FLAG_INTTRAP);
    idt_set_gate(IRQ8, (uint32_t) irq8, __KERNEL_CS_SELECTOR, IDT_FLAG_PRESENT|IDT_FLAG_RING0|IDT_FLAG_32BIT|IDT_FLAG_INTTRAP);
    idt_set_gate(IRQ9, (uint32_t) irq9, __KERNEL_CS_SELECTOR, IDT_FLAG_PRESENT|IDT_FLAG_RING0|IDT_FLAG_32BIT|IDT_FLAG_INTTRAP);
    idt_set_gate(IRQ10, (uint32_t) irq10, __KERNEL_CS_SELECTOR, IDT_FLAG_PRESENT|IDT_FLAG_RING0|IDT_FLAG_32BIT|IDT_FLAG_INTTRAP);
    idt_set_gate(IRQ11, (uint32_t) irq11, __KERNEL_CS_SELECTOR, IDT_FLAG_PRESENT|IDT_FLAG_RING0|IDT_FLAG_32BIT|IDT_FLAG_INTTRAP);
    idt_set_gate(IRQ12, (uint32_t) irq12, __KERNEL_CS_SELECTOR, IDT_FLAG_PRESENT|IDT_FLAG_RING0|IDT_FLAG_32BIT|IDT_FLAG_INTTRAP);
    idt_set_gate(IRQ13, (uint32_t) irq13, __KERNEL_CS_SELECTOR, IDT_FLAG_PRESENT|IDT_FLAG_RING0|IDT_FLAG_32BIT|IDT_FLAG_INTTRAP);
    idt_set_gate(IRQ14, (uint32_t) irq14, __KERNEL_CS_SELECTOR, IDT_FLAG_PRESENT|IDT_FLAG_RING0|IDT_FLAG_32BIT|IDT_FLAG_INTTRAP);
    idt_set_gate(IRQ15, (uint32_t) irq15, __KERNEL_CS_SELECTOR, IDT_FLAG_PRESENT|IDT_FLAG_RING0|IDT_FLAG_32BIT|IDT_FLAG_INTTRAP);
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


//extern isr_t interrupt_handlers[256];


/* Called from our ASM interrupt handler stub */
void irq_handler(struct registers_t *regs) {
    uint32_t irq_nr = regs->int_no - 0x20;

//    if (irq_nr == 16)//FIXME
//        return;

    if (irq_nr != 0 && irq_nr != 1) {
        printk("Calling irq_handler() for IRQ %d (int %d)!\n", irq_nr, irq_nr + 0x20);
    }

    /* Send an EOI (end of interrupt) signal to the PICs. */
    pic_acknowledge(irq_nr);

    if (irq_handlers[irq_nr] != 0) {
        isr_t handler = irq_handlers[irq_nr];
        handler(regs);
    } else
        printk("FAILED HANDLING IRQ %d\n", irq_nr);
}

void irq_init(void) {
    pic_init();
    init_idt();
    isr_install();
    irq_install();
}
