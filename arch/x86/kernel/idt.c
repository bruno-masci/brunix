#include <asm/idt.h>
#include <brunix/stdio.h>


extern void __idt_flush(uint32_t);



static idt_entry_t idt[256] = {[0 ... 255] = {0, 0, 0, 0, 0}};
static idt_ptr_t idt_ptr;




void init_idt() {
    idt_ptr.limit = sizeof(idt_entry_t) * 256 - 1;
    idt_ptr.base = (uint32_t)&idt;

    __idt_flush((uint32_t)&idt_ptr);
}


void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags) {
    idt[num].base_lo = base & 0xFFFF;
    idt[num].base_hi = (base >> 16) & 0xFFFF;

    idt[num].sel     = sel;
    idt[num].always0 = (uint8_t)0;
    // We must uncomment the OR below when we get to using user-mode.
    // It sets the interrupt gate's privilege level to 3.
    idt[num].p_dpl_type   = flags /* | 0x60 */;
}
