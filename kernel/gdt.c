#include <arch/x86/gdt.h>
#include <arch/x86/memlayout.h>     // for VIRT_TO_PHYS

#include <stddef.h>     // for size_t
#include <stdint.h>     // for uint8_t, uint32_t, int32_t
#include <stdbool.h>     // for uint8_t, uint32_t, int32_t

// PROTOTYPES
extern void __gdt_flush(phys_addr_t);

void gdt_set_desc(uint32_t, uint32_t, uint32_t, uint32_t);


__attribute__((__aligned__(4)))
static gdt_desc_t gdt[GDT_ENTRIES];
static gdt_ptr_t gdt_ptr;

//static tss default_tss;


void gdt_init(void) {
    gdt_ptr.limit = (sizeof(gdt_desc_t) * GDT_ENTRIES) - 1;
    gdt_ptr.base = (uint32_t) (VIRT_TO_PHYS_WO(&gdt));
//	gdt_ptr.base = &gdt;

	gdt_set_desc(&gdt, 0, 0, 0);   // Null Segment Descriptor (required)
	gdt_set_desc(&gdt + 8, 0, 0xFFFFFFFF, 0);//0x9a, 0xCF);
    gdt_set_desc(&gdt + 16, 0, 0xFFFFFFFF, 0);//0x92, 0xCF);
//    gdt_set_desc(4, 0, 0xFFFFFFFF, 0xFA, 0xCF);
//    gdt_set_desc(4, 0, 0xFFFFFFFF, 0xF2, 0xCF);
//    gdt_set_desc(4, 0, 0xFFFFFFFF, 0xF2, 0xCF);
//    gdt_set_desc(1, 0, GDT_SEGMENT_LIMIT, GDT_FLAG_RING0 | GDT_FLAG_SEGMENT | GDT_FLAG_CODESEG | GDT_FLAG_PRESENT, GDT_FLAG_4K_GRAN | GDT_FLAG_32_BIT); // Kernel Mode Code Segment Descriptor TODO refactor access flags
//	gdt_set_desc(2, 0, GDT_SEGMENT_LIMIT, GDT_FLAG_RING0 | GDT_FLAG_SEGMENT | GDT_FLAG_DATASEG | GDT_FLAG_PRESENT, GDT_FLAG_4K_GRAN | GDT_FLAG_32_BIT); // Kernel Mode Data segment Descriptor

    /* Flush out the old GDT and install the new changes! */
//    __gdt_flush((uint32_t) (VIRT_TO_PHYS_WO(&gdt_ptr)));

//	gdt_set_desc(3, 0, GDT_SEGMENT_LIMIT, GDT_FLAG_RING3 | GDT_FLAG_SEGMENT | GDT_FLAG_CODESEG | GDT_FLAG_PRESENT, GDT_FLAG_4K_GRAN | GDT_FLAG_32_BIT); // User Mode Code Segment Descriptor
//	gdt_set_desc(4, 0, GDT_SEGMENT_LIMIT, GDT_FLAG_RING3 | GDT_FLAG_SEGMENT | GDT_FLAG_DATASEG | GDT_FLAG_PRESENT, GDT_FLAG_4K_GRAN | GDT_FLAG_32_BIT); // User Mode Data Segment Descriptor


//	default_tss.debug_flag = 0x00;
//	default_tss.io_map = 0x00;
//	default_tss.esp0 = 0x1FFF0;
//	default_tss.ss0 = 0x18;
//	init_gdt_desc(5, (uint32_t) & default_tss, 0x67, 0xE9, 0x00);	/* TSS */
}



/* Setup a descriptor in the Global Descriptor Table */
void gdt_set_desc(uint32_t addr, uint32_t base, uint32_t limit, uint32_t type) { //}, uint8_t flags) {
//    uint32_t esp;
//    asm("movl %%esp, %0" : "=r" (esp) ::);
//    cprintf("addr %x, type %x; %esp %x\n", addr, type, esp);//, flags);
    gdt_desc_t *descriptor = (gdt_desc_t *) addr;
    /* Setup the descriptor base address */
    descriptor->base_low = (base & 0xFFFF);
    descriptor->base_middle = (base >> 16) & 0xFF;
    descriptor->base_high = (base >> 24) & 0xFF;

	/* Setup the descriptor limit */
    descriptor->limit_low = (limit & 0xFFFF);
    descriptor->limit_high = (limit >> 16) & 0xF;

    descriptor->access = (type >> 8) & 0xFF;

	/* Setup the granularity and access flags */
    descriptor->flags = (type & 0xF); // 4 KiB sized blocks
}