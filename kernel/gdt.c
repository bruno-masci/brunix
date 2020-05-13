#include <arch/x86/gdt.h>
#include <arch/x86/memlayout.h>     // for VIRT_TO_PHYS

#include <stddef.h>     // for size_t
#include <stdint.h>     // for uint8_t, uint32_t, int32_t
#include <stdbool.h>     // for uint8_t, uint32_t, int32_t

// PROTOTYPES
extern void __gdt_flush(phys_addr_t);
static void gdt_set_desc(uint32_t, uint32_t, uint32_t, uint8_t, uint8_t);
static void gdt_set_NULL_desc(uint32_t, uint32_t, uint32_t);


__attribute__((__aligned__(4)))
static gdt_desc_t gdt[GDT_ENTRIES];
static gdt_ptr_t gdt_ptr;

//static tss default_tss;


void gdt_init(void) {
    gdt_ptr.limit = (sizeof(gdt_desc_t) * GDT_ENTRIES) - 1;
    gdt_ptr.base = (uint32_t) (VIRT_TO_PHYS_WO(&gdt));
//	gdt_ptr.base = &gdt;

	gdt_set_desc(0, 0, 0, 0, 0);   // Null Segment Descriptor (required)
	gdt_set_desc(1, 0, 0xFFFFFFFF, 0x9a, 0xCF);
    gdt_set_desc(2, 0, 0xFFFFFFFF, 0x92, 0xCF);
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
static void gdt_set_desc(uint32_t num, uint32_t base, uint32_t limit, uint8_t access, uint8_t flags) {
	/* Setup the descriptor base address */
	gdt[num].base_low = (base & 0xFFFF);
	gdt[num].base_middle = (base >> 16) & 0xFF;
	gdt[num].base_high = (base >> 24) & 0xFF;

	/* Setup the descriptor limit */
	gdt[num].limit_low = (limit & 0xFFFF);
	gdt[num].limit_high = (limit >> 16) & 0xF;

//    gdt[num].access.accessed_flag = false;  // the CPU changes this on first access
//    gdt[num].access.readable_flag = true;
//    gdt[num].access.direction_flag = false;
//    gdt[num].access.executable_flag = executable_flag;
//    gdt[num].access.non_system_segment_flag = true;
//    gdt[num].access.dpl = 0;
//    gdt[num].access.present_flag = true;
        gdt[num].access = access;
//FIXME    gdt[num].access = ((struct gdt_desc_access_struct *) access);

	/* Setup the granularity and access flags */
//    gdt[num].flags.size_flag = true;        // 32 bit protected mode
//    gdt[num].flags.granularity_flag = true; // 4 KiB sized blocks
    gdt[num].flags = flags; // 4 KiB sized blocks
}