/**
 * @see Chapter 9 of https://css.csail.mit.edu/6.858/2015/readings/i386.pdf
 */
#include <stdint.h>

#include <brunix/defs.h>
#include <brunix/string.h>
#include <brunix/kernel.h>

#include <asm/processor.h>      // for lidt()
#include <asm/memlayout.h>      // for VIRT_TO_PHYS()
#include <asm/segment.h>        // for __KERNEL_CS_SELECTOR


#define NR_VECTORS 256

#define IDT_FLAG_PRESENT 	0x80

// IDT Gate Descriptor
#define IDT_FLAG_RING0 0x00    /// Interrupt can be called from within RING0
#define IDT_FLAG_RING3 0x60    /// Interrupt can be called from within RING3 (user mode) and lower

#define GATE_TASK 0x05     // unused for now
#define GATE_INTERRUPT 0x0E  // used by "intel" Interrupts (maskable, nonmaskable)
#define GATE_TRAP 0x0F        // used by "Intel" Exceptions (a- faults, traps, aborts; b- programmed -e.g. INT)

struct idt_ptr_struct {
    uint16_t limit;             // Table limit: The upper 16 bits of all selector limits.
    uint32_t base;              // Linear base address: The address of the first gdt_entry_t struct.
} __attribute__((packed));

typedef struct idt_ptr_struct idt_ptr_t;


struct idt_gate_descr_struct {
    uint16_t offset_15_0;       // The lower 16 bits of the address to jump to when this interrupt fires.
    uint16_t cs_selector;       // Kernel code segment selector.
    uint8_t  always0;           // (Not quite true, but we don't use Task Gates. Please check Intel Manual)
    uint8_t flags;              // From lower to higher bits: type(4), dpl(2), present(1).
    uint16_t offset_31_16;      // The upper 16 bits of the address to jump to when this interrupt fires.
} __attribute__((packed));

typedef struct idt_gate_descr_struct idt_entry_t;




//#pragma GCC diagnostic ignored "-Wpedantic"
__attribute__((__aligned__(4)))
PRIVATE idt_entry_t idt_table[NR_VECTORS];

PRIVATE idt_ptr_t idt_ptr;


EXPORT void init_idt(void);
PRIVATE void idt_set_gate(uint8_t num, uint8_t type, uint32_t base, uint16_t cs_selector, uint8_t dpl);
EXPORT void set_intr_gate(unsigned int n, uint32_t addr);
EXPORT void set_trap_gate(unsigned int n, uint32_t addr);

//PRIVATE
//inline
// Interrupts through interrupt gates automatically reset IF, disabling interrupts.
EXPORT void set_intr_gate(unsigned int n, uint32_t addr)   // void *addr TODO
{
    ASSERT(n < 0xFF);
    idt_set_gate((uint8_t) n, GATE_INTERRUPT, addr, __KERNEL_CS_SELECTOR, IDT_FLAG_RING0);
}

EXPORT void set_trap_gate(unsigned int n, uint32_t addr) {
    ASSERT(n < 0xFF);
    idt_set_gate((uint8_t) n, GATE_TRAP, addr, __KERNEL_CS_SELECTOR, IDT_FLAG_RING0);
}

PRIVATE void idt_set_gate(uint8_t num, uint8_t type, uint32_t base, uint16_t cs_selector, uint8_t dpl) {
    // points to the ISR that will handle the interrupt request
    idt_table[num].offset_15_0 = base & 0xFFFF;
    idt_table[num].offset_31_16 = (uint16_t) ((base >> 16) & 0xFFFF);

    /* The segment or 'selector' that this IDT entry will use
	 *  is set here, along with any access flags */
    idt_table[num].cs_selector = cs_selector;
    idt_table[num].always0 = (uint8_t) 0;
    idt_table[num].flags = type | dpl | IDT_FLAG_PRESENT;
}

PRIVATE void idt_flush(phys_addr_t idt_ptr_pa) {
    phys_addr_t p = VIRT_TO_PHYS(idt_ptr_pa);
    lidt(p);
}

EXPORT void init_idt(void) {
    idt_ptr.limit = (sizeof(idt_entry_t) * NR_VECTORS) - 1;
    idt_ptr.base = (uint32_t) idt_table;

    /* Clear out the entire IDT, initializing it to zeros */
    memset((void *) idt_table, 0, sizeof(idt_entry_t) * NR_VECTORS);

    idt_flush((phys_addr_t) &idt_ptr);
}
