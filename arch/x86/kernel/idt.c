#include "idt.h"

#include <asm/processor.h>     // for lidt()
#include <asm/memlayout.h>     // for VIRT_TO_PHYS

#include <brunix/string.h>


#define NR_VECTORS 256


//#pragma GCC diagnostic ignored "-Wpedantic"
__attribute__((__aligned__(4)))
static idt_entry_t idt_table[NR_VECTORS];

static idt_ptr_t idt_ptr;


idt_ptr_t init_idt(void);
void idt_set_gate(uint8_t num, uint8_t type, uint32_t base, uint16_t cs_selector, uint8_t dpl, uint8_t flags);



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

void idt_set_gate(uint8_t num, uint8_t type, uint32_t base, uint16_t cs_selector, uint8_t dpl, uint8_t flags) {
    // points to the ISR that will handle the interrupt request
    idt_table[num].offset_15_0 = base & 0xFFFF;
    idt_table[num].offset_31_16 = (uint16_t) ((base >> 16) & 0xFFFF);

    /* The segment or 'selector' that this IDT entry will use
	 *  is set here, along with any access flags */
    idt_table[num].cs_selector_16 = cs_selector;
    idt_table[num].always0_8 = (uint8_t) 0;
    idt_table[num].flags_8 = flags | type | dpl | IDT_FLAG_PRESENT | IDT_FLAG_RING0;// | IDT_FLAG_32BIT;
}
