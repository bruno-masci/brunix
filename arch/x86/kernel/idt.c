/**
 * @see Chapter 9 of https://css.csail.mit.edu/6.858/2015/readings/i386.pdf
 */
#include <stdint.h>             // for uintN_t

#include <brunix/defs.h>        // for PRIVATE, IMPORT, EXPORT
#include <brunix/string.h>
#include <brunix/kernel.h>

#include <asm/processor.h>      // for lidt()
#include <asm/memlayout.h>      // for VIRT_TO_PHYS()
#include <asm/segment.h>        // for __KERNEL_CS_SELECTOR


#define NR_VECTORS 256

// See Figure 9-3 from 80386 manual
typedef enum {
    GATE_TASK = 0x5,            // Unused for now.
    GATE_INTERRUPT = 0xE,       // Used by Interrupts (maskable, nonmaskable)
    GATE_TRAP = 0xF             // Used by Exceptions (a- faults, traps, aborts; b- programmed -e.g. INT)
} idt_gate_descr_type_enum;

// See Figure 9-1 from 80386 manual
struct idt_ptr_struct {
    uint16_t limit;             // Table limit (size - 1)
    uint32_t base;              // Table's linear base address.
} __attribute__((packed));
typedef struct idt_ptr_struct idt_ptr_t;

// See Figure 9-3 from 80386 manual
struct idt_gate_descr_struct {
    uint16_t offset_15_0;       // The lower 16 bits of the address to jump to (the handler) when this interrupt fires.
    uint16_t selector;          // Code Segment selector.
    uint8_t always_zero_8;      // (Not quite true, but it is effectively the same. Please check Intel Manual)
    uint8_t type : 4;           // Gate type: task, trap, interrupt.
    uint8_t always_zero : 1;
    uint8_t dpl : 2;            // Descriptor Privilege Level.
    uint8_t present : 1;
    uint16_t offset_31_16;      // The upper 16 bits of the address to jump to (the handler) when this interrupt fires.
} __attribute__((packed));
typedef struct idt_gate_descr_struct idt_gate_descr_t;


__attribute__((__aligned__(4)))
PRIVATE idt_gate_descr_t idt_table[NR_VECTORS];

PRIVATE idt_ptr_t idt_ptr;


EXPORT void init_idt(void);
EXPORT void set_intr_gate(unsigned int n, uint32_t addr);
EXPORT void set_trap_gate(unsigned int n, uint32_t addr);
PRIVATE void idt_set_gate(uint8_t num, idt_gate_descr_type_enum type, uint32_t base, uint16_t cs_selector, segment_privilege_level_enum dpl);



/*
 * Interrupts through interrupt gates automatically reset IF, disabling interrupts.
 */

//inline
void set_intr_gate(unsigned int n, uint32_t addr)
{
    ASSERT(n <= 255);   //TODO ver tema 256 q no anda
    idt_set_gate((uint8_t) n, GATE_INTERRUPT, (uint32_t) addr, __KERNEL_CS_SELECTOR, PRIVILEGE_LEVEL_KERNEL);
}

EXPORT void set_trap_gate(unsigned int n, uint32_t addr) {
    ASSERT(n < 0xFF);
    idt_set_gate((uint8_t) n, GATE_TRAP, (uint32_t) addr, __KERNEL_CS_SELECTOR, PRIVILEGE_LEVEL_KERNEL);
}

PRIVATE void idt_set_gate(uint8_t num, idt_gate_descr_type_enum type, uint32_t base, uint16_t cs_selector, segment_privilege_level_enum dpl) {
    idt_table[num].offset_15_0 = base & 0xFFFF;
    idt_table[num].offset_31_16 = (uint16_t) ((base >> 16) & 0xFFFF);
    idt_table[num].selector = cs_selector;
    idt_table[num].always_zero_8 = (uint8_t) 0;
    idt_table[num].type = type;
    idt_table[num].always_zero = 0;
    idt_table[num].dpl = dpl;
    idt_table[num].present = true;
}

PRIVATE void idt_flush(phys_addr_t idt_ptr_pa) {
    phys_addr_t p = VIRT_TO_PHYS(idt_ptr_pa);
    lidt(p);
}

EXPORT void init_idt(void) {
    uint16_t table_size = sizeof(idt_gate_descr_t) * NR_VECTORS;
    idt_ptr.limit = (uint16_t) (table_size - 1);
    idt_ptr.base = (uint32_t) idt_table;

    /* Clear out the entire IDT, initializing it to zeros */
    memset((void *) idt_table, 0, table_size);

    idt_flush((phys_addr_t) &idt_ptr);
}
