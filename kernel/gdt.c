#include <stdint.h>                 // for uint32_t

#include <arch/x86/processor.h>     // for lgdt()
#include <arch/x86/gdt.h>
#include <arch/x86/memlayout.h>     // for VIRT_TO_PHYS, phys_addr_t
#include <arch/x86/segment.h>       // for __KERNEL_CS_SELECTOR, __KERNEL_DS_SELECTOR

#include <brunix/defs.h>            // for PRIVATE, INIT_FUNC




__attribute__((__aligned__(4)))
PRIVATE gdt_desc_t gdt[GDT_ENTRIES];

PRIVATE gdt_ptr_t gdt_ptr;


// <<<<<<<<<<<<<<<<<<<<<<< PROTOTYPES >>>>>>>>>>>>>>>>>>>>>>>
PRIVATE void gdt_set_desc(phys_addr_t, uint32_t, uint32_t, uint32_t, uint32_t);
PRIVATE void gdt_flush(phys_addr_t);



INIT_FUNC void gdt_init(void) {
    gdt_ptr.limit = (sizeof(gdt_desc_t) * GDT_ENTRIES) - 1;
    gdt_ptr.base = (uint32_t) (VIRT_TO_PHYS_WO(&gdt));

    // Null Segment Descriptor (required)
    gdt_set_desc(
            (phys_addr_t) &gdt[0],
            0,
            0,
            0,
            0
    );

    // Kernel Mode Code Segment Descriptor
    gdt_set_desc(
            (phys_addr_t) &gdt[__KERNEL_CS_SELECTOR >> 3],
            0,
            GDT_SEGMENT_LIMIT,
            GDT_FLAG_RING0 | GDT_FLAG_SEGMENT | GDT_FLAG_CODESEG | GDT_FLAG_PRESENT,
            0xC //GDT_FLAG_4K_GRAN | GDT_FLAG_32_BIT // es 0xC o 0xCF??
    );

    // Kernel Mode Data Segment Descriptor
    gdt_set_desc(
            (phys_addr_t) &gdt[__KERNEL_DS_SELECTOR >> 3],
            0,
            GDT_SEGMENT_LIMIT,
            GDT_FLAG_RING0 | GDT_FLAG_SEGMENT | GDT_FLAG_DATASEG | GDT_FLAG_PRESENT, //            0x92,
            0xC //FIXME GDT_FLAG_4K_GRAN | GDT_FLAG_32_BIT
    );

    // Flush out the old GDT and install the new changes!
    gdt_flush((phys_addr_t) (VIRT_TO_PHYS_WO(&gdt_ptr)));
}

// Setup a descriptor in the Global Descriptor Table
PRIVATE void gdt_set_desc(phys_addr_t addr, uint32_t base, uint32_t limit_20, uint32_t access_8, uint32_t flags_4) {
    gdt_desc_t *descriptor = (gdt_desc_t *) addr;

    // Setup the descriptor base address
    descriptor->base_low_16 = (base & 0xFFFF);
    descriptor->base_middle_8 = (base >> 16) & 0xFF;
    descriptor->base_high_8 = (base >> 24) & 0xFF;

	// Setup the descriptor limit
    descriptor->limit_low_16 = (limit_20 & 0xFFFF);
    descriptor->limit_high_4 = (limit_20 >> 16) & 0xF;

    descriptor->access_8 = access_8 & 0xFF;

	// Setup the granularity and access flags
    descriptor->flags_4 = flags_4 & 0xF; // 4 KiB sized blocks
}

PRIVATE void gdt_flush(phys_addr_t addr) {
    lgdt(addr);

    // The kernel uses DS, ES and SS.
    asm volatile("movw %%ax,%%ds" : : "a" (__KERNEL_DS_SELECTOR));
    asm volatile("movw %%ax,%%es" : : "a" (__KERNEL_DS_SELECTOR));
    asm volatile("movw %%ax,%%ss" : : "a" (__KERNEL_DS_SELECTOR));

    // The kernel never uses FS or GS -> leave those set to zero
    asm volatile("movw %%ax,%%fs" : : "a" (0));
    asm volatile("movw %%ax,%%gs" : : "a" (0));

    // Load the kernel text segment into CS with a long jump.
    asm volatile("ljmp %0,$1f\n 1:\n" : : "i" (__KERNEL_CS_SELECTOR));
}
