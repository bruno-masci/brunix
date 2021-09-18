#include "idt.h"

#include <asm/processor.h>     // for lidt()
#include <asm/memlayout.h>     // for VIRT_TO_PHYS
#include <asm/segment.h>     // for __KERNEL_CS_SELECTOR.

#include <brunix/defs.h>
#include <brunix/string.h>
#include <brunix/kernel.h>


#define NR_VECTORS 256

#define IDT_FLAG_PRESENT 	0x80
/// Interrupt can be called from within RING0
#define IDT_FLAG_RING0		0x00
/// Interrupt can be called from within RING3 (user mode) and lower
#define IDT_FLAG_RING3		0x60

//#pragma GCC diagnostic ignored "-Wpedantic"
__attribute__((__aligned__(4)))
static idt_entry_t idt_table[NR_VECTORS];

static idt_ptr_t idt_ptr;


idt_ptr_t init_idt(void);
PRIVATE void idt_set_gate(uint8_t num, uint8_t type, uint32_t base, uint16_t cs_selector, uint8_t dpl);


enum idt_gate_descr_type {
    GATE_TASK = 0x05,     // unused for now
    GATE_INTERRUPT = 0x0E,  // used by interrupts
    GATE_TRAP = 0x0F        // used by Exceptions
};


//static
//inline
// Interrupts through interrupt gates automatically reset IF, disabling interrupts.
void set_intr_gate(unsigned int n, uint32_t addr)   // void *addr TODO
{
    ASSERT(n < 0xFF);
    idt_set_gate((uint8_t) n, GATE_INTERRUPT, addr, __KERNEL_CS_SELECTOR, IDT_FLAG_RING0);
}

void set_trap_gate(unsigned int n, uint32_t addr) {
    ASSERT(n < 0xFF);
    idt_set_gate((uint8_t) n, GATE_TRAP, addr, __KERNEL_CS_SELECTOR, IDT_FLAG_RING0);
}


static void idt_flush(phys_addr_t idt_ptr_pa) {
    phys_addr_t p = VIRT_TO_PHYS(idt_ptr_pa);
    lidt(p);
}

idt_ptr_t init_idt(void) {
    idt_ptr.limit = (sizeof(idt_entry_t) * NR_VECTORS) - 1;
    idt_ptr.base = (uint32_t) idt_table;

    /* Clear out the entire IDT, initializing it to zeros */
    memset((void *) idt_table, 0, sizeof(idt_entry_t) * NR_VECTORS);

    idt_flush((phys_addr_t) &idt_ptr);

    return idt_ptr;
}

PRIVATE void idt_set_gate(uint8_t num, uint8_t type, uint32_t base, uint16_t cs_selector, uint8_t dpl) {
    // points to the ISR that will handle the interrupt request
    idt_table[num].offset_15_0 = base & 0xFFFF;
    idt_table[num].offset_31_16 = (uint16_t) ((base >> 16) & 0xFFFF);

    /* The segment or 'selector' that this IDT entry will use
	 *  is set here, along with any access flags */
    idt_table[num].cs_selector_16 = cs_selector;
    idt_table[num].always0_8 = (uint8_t) 0;
    idt_table[num].flags_8 = type | dpl | IDT_FLAG_PRESENT;
}
