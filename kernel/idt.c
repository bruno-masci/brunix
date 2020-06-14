#include <arch/x86/processor.h>     // for lidt()
#include <arch/x86/idt.h>
#include <arch/x86/mmu.h>
#include <arch/x86/memlayout.h>     // for VIRT_TO_PHYS


void __idt_flush(uint32_t idtptr) {
    lidt(idtptr);
}

//#pragma GCC diagnostic ignored "-Wpedantic"
//__attribute__((__aligned__(4)))
static idt_entry_t idt[256];// = {[0 ... 255] = {0, 0, 0, 0, 0, 0, 0, 0}};

static idt_ptr_t idt_ptr;




void init_idt(void) {
    idt_ptr.limit = (sizeof(idt_entry_t) * 256) - 1;
    idt_ptr.base = (uint32_t)(VIRT_TO_PHYS_WO(&idt));

    /* Clear out the entire IDT, initializing it to zeros */
//    memset(&idt[0], 0, sizeof(idt_entry_t) * 256); FIXME check
}

void idt_flush(void) {
    __idt_flush((uint32_t) (VIRT_TO_PHYS_WO(&idt_ptr)));
}

void idt_set_gate(uint8_t num, uint32_t base, uint16_t cs_selector, uint8_t dpl) {
    // points to the ISR that will handle the interrupt request
    idt[num].base_15_0 = base & 0xFFFF;
    idt[num].base_31_16 = (base >> 16) & 0xFFFF;

    idt[num].cs_selector_16 = cs_selector;
//    idt[num].args_5 = 0;
    idt[num].always0_8 = (uint8_t) 0;

//    idt[num].type = (istrap) ? STS_TG32 : STS_IG32;
    idt[num].type_4 = STS_IG32;
    idt[num].s_1 = 0;
    idt[num].dpl_2 = dpl;
    idt[num].p_1 = 1;
}