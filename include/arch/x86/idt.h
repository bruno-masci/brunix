#ifndef __ARCH_IDT_H__
#define __ARCH_IDT_H__

#include <stdint.h>


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