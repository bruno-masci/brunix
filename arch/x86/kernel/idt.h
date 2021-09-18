#ifndef __ARCH_IDT_H__
#define __ARCH_IDT_H__

#include <stdint.h>


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
#define IDT_FLAG_16BIT		0x00/
/// Size of gate is 32 bit
//#define IDT_FLAG_32BIT		0x08



struct idt_ptr_struct {
    uint16_t limit;               // Table limit: The upper 16 bits of all selector limits.
    uint32_t base;                // Linear base address: The address of the first gdt_entry_t struct.
} __attribute__((packed));
typedef struct idt_ptr_struct idt_ptr_t;


// A struct describing an interrupt gate.
struct idt_gate_descr_struct
{
    uint16_t offset_15_0;             // The lower 16 bits of the address to jump to when this interrupt fires.
    uint16_t cs_selector_16;                 // Kernel segment selector.
    uint8_t  always0_8;             // Not quite true, but we don't use Task Gates (see Intel Manual).
    uint8_t flags_8;                  // from low to high: type(4), dpl(2), present(1) -> |TYPE|0|DPL|P|
    uint16_t offset_31_16;             // The upper 16 bits of the address to jump to.
} __attribute__((packed));
typedef struct idt_gate_descr_struct idt_entry_t;


#endif /* __ARCH_IDT_H__ */
