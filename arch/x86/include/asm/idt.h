#ifndef __ARCH_IDT_H__
#define __ARCH_IDT_H__

#include <brunix/stddef.h>


struct idt_ptr_struct {
    uint16_t limit;               // Table limit: The upper 16 bits of all selector limits.
    uint32_t base;                // Linear base address: The address of the first gdt_entry_t struct.
} __attribute__((packed));
typedef struct idt_ptr_struct idt_ptr_t;





// Initialisation function is publicly accessible.
void init_idt(void);
void idt_set_gate(uint8_t, uint32_t, uint16_t, uint8_t);




// A struct describing an interrupt gate.
struct idt_entry_struct
{
    uint16_t base_lo;             // The lower 16 bits of the address to jump to when this interrupt fires.
    uint16_t sel;                 // Kernel segment selector.
    uint8_t  always0;             // This must always be zero.
    uint8_t  p_dpl_type;          // |P|DL|0|TYPE|
    uint16_t base_hi;             // The upper 16 bits of the address to jump to.
} __attribute__((packed));
typedef struct idt_entry_struct idt_entry_t;
//struct gatedesc_s {
//  u16_t offset_low;
//  u16_t selector;
//  u8_t pad;                     /* |000|XXXXX| ig & trpg, |XXXXXXXX| task g */
//  u8_t p_dpl_type;              /* |P|DL|0|TYPE| */
//  u16_t offset_high;
//} __attribute__((packed));




extern void irq0();
extern void irq1();
extern void irq2();
extern void irq3();
extern void irq4();
extern void irq5();
extern void irq6();
extern void irq7();
extern void irq8();
extern void irq9();
extern void irq10();
extern void irq11();
extern void irq12();
extern void irq13();
extern void irq14();
extern void irq15();




#endif /* __ARCH_IDT_H__ */
