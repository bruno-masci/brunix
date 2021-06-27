#include <stdint.h>                 // for uint32_t

#include <arch/x86/processor.h>     // for lgdt()
#include <arch/x86/gdt.h>
#include <arch/x86/memlayout.h>     // for VIRT_TO_PHYS, phys_addr_t
#include <arch/x86/segment.h>       // for __KERNEL_CS_SELECTOR, __KERNEL_DS_SELECTOR

#include <brunix/defs.h>            // for PRIVATE, INIT_FUNC
#include <brunix/kernel.h>


INIT_FUNC void gdt_init(void);



__attribute__((__aligned__(4)))
PRIVATE struct gdt_desc_struct gdt[GDT_ENTRIES];

PRIVATE struct gdt_ptr_struct gdt_ptr;


// <<<<<<<<<<<<<<<<<<<<<<< PROTOTYPES >>>>>>>>>>>>>>>>>>>>>>>
PRIVATE void gdt_set_desc(phys_addr_t, uint32_t, uint32_t, uint32_t, uint32_t);
PRIVATE void gdt_flush(phys_addr_t);

PRIVATE void print_segment_selectors(void);


struct gdt_ptr_struct get_gdt_ptr(uint16_t total_gdt_entries, uint32_t gdt_base_addr) {
    gdt_ptr.limit = (sizeof(struct gdt_desc_struct) * total_gdt_entries) - 1;
    gdt_ptr.base = (uint32_t) (VIRT_TO_PHYS_WO(gdt_base_addr));
    return gdt_ptr;
}

PRIVATE  void print_segment_selectors(void) {
    uint32_t cs;
    uint32_t ds;
    uint32_t es;
    uint32_t ss;
    uint32_t fs;
    uint32_t gs;
    asm("movl %%cs, %0" : "=r" (cs) ::);
    asm("movl %%ds, %0" : "=r" (ds) ::);
    asm("movl %%es, %0" : "=r" (es) ::);
    asm("movl %%ss, %0" : "=r" (ss) ::);
    asm("movl %%fs, %0" : "=r" (fs) ::);
    asm("movl %%gs, %0" : "=r" (gs) ::);
    printk("CS 0x%x, DS 0x%x, ES 0x%x, SS 0x%x, FS 0x%x, GS 0x%x\n", cs, ds, es, ss, fs, gs);
}

INIT_FUNC void gdt_init(void) {
    gdt_ptr = get_gdt_ptr(GDT_ENTRIES, &gdt);

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

    printk("Segment selectors (bootloader) -> ");
    print_segment_selectors();

    // Flush out the old GDT and install the new changes!
    gdt_flush((phys_addr_t) (VIRT_TO_PHYS_WO(&gdt_ptr)));

    printk("Segment selectors (brunix) -> ");
    print_segment_selectors();
    printk("\n");
}

void fill_gdt_desc(struct gdt_desc_struct *descriptor, uint32_t base, uint32_t limit_20, uint32_t access_8, uint32_t flags_4) {
    // Setup the descriptor base address
    descriptor->base_low_16 = (base & 0xFFFF);
    descriptor->base_middle_8 = (base >> 16) & 0xFF;
    descriptor->base_high_8 = (uint8_t) ((base >> 24) & 0xFF);

    // Setup the descriptor limit
    descriptor->limit_low_16 = (limit_20 & 0xFFFF);
    descriptor->limit_high_4 = (limit_20 >> 16) & 0xF;

    descriptor->access_8 = access_8 & 0xFF;

    // Setup the granularity and access flags
    descriptor->flags_4 = flags_4 & 0xF; // 4 KiB sized blocks
}

// Setup a descriptor in the Global Descriptor Table
PRIVATE void gdt_set_desc(phys_addr_t addr, uint32_t base, uint32_t limit_20, uint32_t access_8, uint32_t flags_4) {
    struct gdt_desc_struct *descriptor = (struct gdt_desc_struct *) addr;

    fill_gdt_desc(descriptor, base, limit_20, access_8, flags_4);
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
