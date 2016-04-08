#include <asm/gdt.h>
#include <brunix/stdio.h>

// PROTOTYPES
extern void gdt_flush(uint32_t);
static void gdt_set_gate(int32_t, uint32_t, uint32_t, uint8_t, uint8_t);


static gdt_entry_t gdt[GDT_ENTRIES] = {[0 ... GDT_ENTRIES-1] = {0, 0, 0, 0, 0, 0}};
static gdt_ptr_t gdt_ptr;



void gdt_init() {
	gdt_ptr.limit = (sizeof(gdt_entry_t) * GDT_ENTRIES) - 1;
	gdt_ptr.base = (size_t)&gdt;

	//memset(&gdt, 0, sizeof(gdt_entry_t) * 5);

	gdt_set_gate(0, 0, 0, 0, 0);                // Null Segment Descriptor (required)
	gdt_set_gate(1, 0, 0xFFFFFFFF, GDT_FLAG_RING0 | GDT_FLAG_SEGMENT | GDT_FLAG_CODESEG | GDT_FLAG_PRESENT, GDT_FLAG_4K_GRAN | GDT_FLAG_32_BIT); // Kernel Mode Code Segment Descriptor
	gdt_set_gate(2, 0, 0xFFFFFFFF, GDT_FLAG_RING0 | GDT_FLAG_SEGMENT | GDT_FLAG_DATASEG | GDT_FLAG_PRESENT, GDT_FLAG_4K_GRAN | GDT_FLAG_32_BIT); // Kernel Mode Data segment Descriptor
	gdt_set_gate(3, 0, 0xFFFFFFFF, GDT_FLAG_RING3 | GDT_FLAG_SEGMENT | GDT_FLAG_CODESEG | GDT_FLAG_PRESENT, GDT_FLAG_4K_GRAN | GDT_FLAG_32_BIT); // User Mode Code Segment Descriptor
	gdt_set_gate(4, 0, 0xFFFFFFFF, GDT_FLAG_RING3 | GDT_FLAG_SEGMENT | GDT_FLAG_DATASEG | GDT_FLAG_PRESENT, GDT_FLAG_4K_GRAN | GDT_FLAG_32_BIT); // User Mode Data Segment Descriptor

	//TODO ver gdt.c en eduOs para TSS (gate 5)

	/* Flush out the old GDT and install the new changes! */
	gdt_flush((uint32_t)&gdt_ptr);
}



/* Setup a descriptor in the Global Descriptor Table */
static void gdt_set_gate(int32_t num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran) {

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
