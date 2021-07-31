#ifndef __ARCH_IDT_H__
#define __ARCH_IDT_H__

#include <stdint.h>


struct idt_ptr_struct {
    uint16_t limit;               // Table limit: The upper 16 bits of all selector limits.
    uint32_t base;                // Linear base address: The address of the first gdt_entry_t struct.
} __attribute__((packed));
typedef struct idt_ptr_struct idt_ptr_t;





// Initialisation function is publicly accessible.
idt_ptr_t init_idt(void);
void idt_set_gate(uint8_t num, uint32_t base, uint16_t cs_selector, uint8_t dpl);




// A struct describing an interrupt gate.
//struct idt_entry_struct
//{
//    uint16_t base_lo;             // The lower 16 bits of the address to jump to when this interrupt fires.
//    uint16_t sel;                 // Kernel segment selector.
//    uint8_t  always0;             // This must always be zero.
//    uint8_t  p_dpl_type;          // |P|DL|0|TYPE|
//    uint16_t base_hi;             // The upper 16 bits of the address to jump to.
//} __attribute__((packed));
struct idt_entry_struct
{
    uint16_t base_15_0;             // The lower 16 bits of the address to jump to when this interrupt fires.
    uint16_t cs_selector_16;                 // Kernel segment selector.
//    uint32_t args_5 : 5;              // # args, 0 for interrupt/trap gates
    uint8_t  always0_8;             // This must always be zero.
    uint32_t type_4 : 4;        // type(STS_{IG32,TG32})
    uint32_t s_1 : 1;           // must be 0 (system)
    uint32_t dpl_2 : 2;         // descriptor(meaning new) privilege level
    uint32_t p_1 : 1;           // Present
    uint16_t base_31_16;             // The upper 16 bits of the address to jump to.
} __attribute__((packed));
typedef struct idt_entry_struct idt_entry_t;
//struct gatedesc_s {
//  u16_t offset_low;
//  u16_t selector;
//  u8_t pad;                     /* |000|XXXXX| ig & trpg, |XXXXXXXX| task g */
//  u8_t p_dpl_type;              /* |P|DL|0|TYPE| */
//  u16_t offset_high;
//} __attribute__((packed));




//void irq0(void);
//void irq1(void);
//void irq2(void);
//void irq3(void);
//void irq4(void);
//void irq5(void);
//void irq6(void);
//void irq7(void);
//void irq8(void);
//void irq9(void);
//void irq10(void);
//void irq11(void);
//void irq12(void);
//void irq13(void);
//void irq14(void);
//void irq15(void);




#endif /* __ARCH_IDT_H__ */
