#include <arch/x86/processor.h>     // for lgdt(), ltr()
#include <arch/x86/gdt.h>
#include <arch/x86/memlayout.h>     // for VIRT_TO_PHYS
#include <arch/x86/segment.h>     // for __KERNEL_DS_SELECTOR

#include <brunix/defs.h>        // for PRIVATE

#include <stddef.h>     // for size_t
#include <stdint.h>     // for uint8_t, uint32_t, int32_t
#include <stdbool.h>    // for uint8_t, uint32_t, int32_t


PRIVATE void __gdt_flush(phys_addr_t val) {
    lgdt(val);

    // The kernel uses DS, ES and SS.
    asm volatile("movw %%ax,%%ds" : : "a" (__KERNEL_DS_SELECTOR));
    asm volatile("movw %%ax,%%es" : : "a" (__KERNEL_DS_SELECTOR));
    asm volatile("movw %%ax,%%ss" : : "a" (__KERNEL_DS_SELECTOR));

    // The kernel never uses FS or GS, so we leave those set to
    // zero until we suport user mode. // TODO ...the user data segment.
    asm volatile("movw %%ax,%%fs" : : "a" (0)); //__USER_DS_SELECTOR | 3
    asm volatile("movw %%ax,%%gs" : : "a" (0)); //__USER_DS_SELECTOR | 3

    // Load the kernel text segment into CS with a long jump.
    asm volatile("ljmp %0,$1f\n 1:\n" : : "i" (__KERNEL_CS_SELECTOR));
}

PRIVATE void __tss_flush(uint16_t selector) {
    ltr(selector);
}


void gdt_set_desc(phys_addr_t, uint32_t, uint32_t, uint32_t, uint32_t);


__attribute__((__aligned__(4)))
static gdt_desc_t gdt[GDT_ENTRIES];
static gdt_ptr_t gdt_ptr;



//static tss_t default_tss = {
//    debug_flag: 0x00,
//    io_map: 0x00,
//    esp0: 0,            //0x1FFF0,
//    ss0: __KERNEL_DS_SELECTOR  //Kernel DS       //0x18;
//    //	init_gdt_desc(5, (uint32_t) & default_tss, 0x67, 0xE9, 0x00);	/* TSS */
//};



void gdt_init(void) {
    gdt_ptr.limit = (sizeof(gdt_desc_t) * GDT_ENTRIES) - 1;
    gdt_ptr.base = (uint32_t) (VIRT_TO_PHYS_WO(&gdt));

//    cprintf("base=%x, limit = %d\n", gdt_ptr.base, gdt_ptr.limit);

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

//    gdt_set_desc(&gdt[3], (uint32_t) &default_tss, &default_tss + sizeof(default_tss), 0xE9, 0x0);	/* TSS */
//    memset(&default_tss, 0, sizeof(tss_t));

    uint16_t sel = 0x18;
//    __tss_flush(sel);

    /* Flush out the old GDT and install the new changes! */
    __gdt_flush((uint32_t) (VIRT_TO_PHYS_WO(&gdt_ptr)));


//	default_tss.debug_flag = 0x00;
//	default_tss.io_map = 0x00;
//	default_tss.esp0 = 0x1FFF0;
//	default_tss.ss0 = 0x18;
//	init_gdt_desc(5, (uint32_t) & default_tss, 0x67, 0xE9, 0x00);	/* TSS */
}



/* Setup a descriptor in the Global Descriptor Table */
void gdt_set_desc(phys_addr_t addr, uint32_t base, uint32_t limit_20, uint32_t access_8, uint32_t flags_4) {
    gdt_desc_t *descriptor = (gdt_desc_t *) addr;

    /* Setup the descriptor base address */
    descriptor->base_low_16 = (base & 0xFFFF);
    descriptor->base_middle_8 = (base >> 16) & 0xFF;
    descriptor->base_high_8 = (base >> 24) & 0xFF;

	/* Setup the descriptor limit */
    descriptor->limit_low_16 = (limit_20 & 0xFFFF);
    descriptor->limit_high_4 = (limit_20 >> 16) & 0xF;

    descriptor->access_8 = access_8 & 0xFF;

	/* Setup the granularity and access flags */
    descriptor->flags_4 = flags_4 & 0xF; // 4 KiB sized blocks

//    cprintf("descr gdt_addr %p, limit_low %p limit_high %p access %p, flags %p\n", addr,
//        descriptor->limit_low, descriptor->limit_high, descriptor->access, descriptor->flags);
}