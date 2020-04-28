#include <arch/x86/gdt.h>
#include <arch/x86/memlayout.h>     // for VIRT_TO_PHYS

#include <stddef.h>     // for size_t
#include <stdint.h>     // for uint8_t, uint32_t, int32_t

// PROTOTYPES
extern void __gdt_flush(physaddr_t);
static void gdt_set_desc(uint32_t, uint32_t, uint32_t, uint8_t, uint8_t);


#pragma GCC diagnostic ignored "-Wpedantic"
__attribute__((__aligned__(4)))
static gdt_desc_t gdt[GDT_ENTRIES] = {[0 ... GDT_ENTRIES-1] = {0, 0, 0, 0, 0, 0}};
static gdt_ptr_t gdt_ptr;
//static tss default_tss;


void gdt_init(void) {
	gdt_ptr.limit = (sizeof(gdt_desc_t) * GDT_ENTRIES) - 1;
	gdt_ptr.base = (size_t)(VIRT_TO_PHYS(&gdt));
//	gdt_ptr.base = (uint32_t)&gdt;

	gdt_set_desc(0, 0, 0, 0, 0);                // Null Segment Descriptor (required)
	gdt_set_desc(1, 0, GDT_SEGMENT_LIMIT, GDT_FLAG_RING0 | GDT_FLAG_SEGMENT | GDT_FLAG_CODESEG | GDT_FLAG_PRESENT, GDT_FLAG_4K_GRAN | GDT_FLAG_32_BIT); // Kernel Mode Code Segment Descriptor TODO refactor access flags
	gdt_set_desc(2, 0, GDT_SEGMENT_LIMIT, GDT_FLAG_RING0 | GDT_FLAG_SEGMENT | GDT_FLAG_DATASEG | GDT_FLAG_PRESENT, GDT_FLAG_4K_GRAN | GDT_FLAG_32_BIT); // Kernel Mode Data segment Descriptor
	gdt_set_desc(3, 0, GDT_SEGMENT_LIMIT, GDT_FLAG_RING3 | GDT_FLAG_SEGMENT | GDT_FLAG_CODESEG | GDT_FLAG_PRESENT, GDT_FLAG_4K_GRAN | GDT_FLAG_32_BIT); // User Mode Code Segment Descriptor
	gdt_set_desc(4, 0, GDT_SEGMENT_LIMIT, GDT_FLAG_RING3 | GDT_FLAG_SEGMENT | GDT_FLAG_DATASEG | GDT_FLAG_PRESENT, GDT_FLAG_4K_GRAN | GDT_FLAG_32_BIT); // User Mode Data Segment Descriptor


//	default_tss.debug_flag = 0x00;
//	default_tss.io_map = 0x00;
//	default_tss.esp0 = 0x1FFF0;
//	default_tss.ss0 = 0x18;
//	init_gdt_desc(5, (uint32_t) & default_tss, 0x67, 0xE9, 0x00);	/* TSS */

	/* Flush out the old GDT and install the new changes! */
	__gdt_flush((uint32_t)(VIRT_TO_PHYS(&gdt_ptr)));
}



/* Setup a descriptor in the Global Descriptor Table */
static void gdt_set_desc(uint32_t num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran) {
	/* Setup the descriptor base address */
	gdt[num].base_low = (base & 0xFFFF);
	gdt[num].base_middle = (base >> 16) & 0xFF;
	gdt[num].base_high = (base >> 24) & 0xFF;

	/* Setup the descriptor limits */
	gdt[num].limit_low = (limit & 0xFFFF);
    gdt[num].granularity = (limit >> 16) & 0x0F;

	/* Setup the granularity and access flags */
    gdt[num].granularity |= gran & 0xF0;
	gdt[num].access = access;
}