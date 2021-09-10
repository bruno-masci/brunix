#include "idt.h"

#include <asm/processor.h>     // for lidt()
#include <asm/memlayout.h>     // for VIRT_TO_PHYS

#include <brunix/string.h>


#define IDT_TOTAL_ENTRIES 256


//#pragma GCC diagnostic ignored "-Wpedantic"
__attribute__((__aligned__(4)))
static idt_entry_t idt[IDT_TOTAL_ENTRIES];

static idt_ptr_t idt_ptr;


static void idt_flush(phys_addr_t idt_ptr) {
    phys_addr_t p = VIRT_TO_PHYS(idt_ptr);
    lidt(p);
}

idt_ptr_t init_idt(void) {
    idt_ptr.limit = (sizeof(idt_entry_t) * IDT_TOTAL_ENTRIES) - 1;
    idt_ptr.base = (uint32_t) idt;

    /* Clear out the entire IDT, initializing it to zeros */
    memset((void *) idt, 0, sizeof(idt_entry_t) * IDT_TOTAL_ENTRIES);

    idt_flush((phys_addr_t) &idt_ptr);

    return idt_ptr;
}

void idt_set_gate(uint8_t num, uint32_t base, uint16_t cs_selector, uint8_t flags) {
    // points to the ISR that will handle the interrupt request
    idt[num].base_15_0 = base & 0xFFFF;
    idt[num].base_31_16 = (uint16_t) ((base >> 16) & 0xFFFF);

    /* The segment or 'selector' that this IDT entry will use
	 *  is set here, along with any access flags */
    idt[num].cs_selector_16 = cs_selector;
    idt[num].always0_8 = (uint8_t) 0;
    idt[num].flags = flags;
}
