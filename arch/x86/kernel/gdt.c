#include <asm/gdt.h>
#include <asm/segment.h>


// PROTOTYPES
void gdt_init(void);
extern void __gdt_flush(uint32_t);
/*static */ void gdt_fill_table(struct gdt_desc_struct *gdt_table, struct tss *initial_tss);
/*static */ void gdt_set_descr(struct gdt_desc_struct *, uint32_t, uint32_t, uint8_t, uint8_t);
void gdt_fill_default_tss(struct tss *default_tss);

static struct tss default_tss;

struct gdt_desc_struct gdt_table[GDT_ENTRIES];


void gdt_init(void) {
    gdt_fill_default_tss(&default_tss);
    gdt_fill_table(gdt_table, &default_tss);
}

void gdt_fill_default_tss(struct tss *tss) {
    tss->debug_flag = 0x00;
    //	tss.io_map = 0x0;  // TODO hace falta?
    tss->esp0 = 0; //TODO
    tss->ss0 = __KERNEL_DS_SELECTOR;
    //	tss.io_map = (uint16_t) sizeof(default_tss);
}

/*static */ void gdt_fill_table(struct gdt_desc_struct *gdt_table_ptr, struct tss *initial_tss) {
    gdt_set_descr(&gdt_table_ptr[0], 0, 0, 0, 0);                // Null Segment Descriptor (required)
    gdt_set_descr(&gdt_table_ptr[1], 0, GDT_SEGMENT_LIMIT, GDT_FLAG_RING0 | GDT_FLAG_CODE_DATA_SEGMENT | GDT_FLAG_CODESEG | GDT_FLAG_PRESENT, GDT_FLAG_4K_GRAN | GDT_FLAG_32_BIT); // Kernel Mode Code Segment Descriptor TODO refactor access flags
    gdt_set_descr(&gdt_table_ptr[2], 0, GDT_SEGMENT_LIMIT, GDT_FLAG_RING0 | GDT_FLAG_CODE_DATA_SEGMENT | GDT_FLAG_DATASEG | GDT_FLAG_PRESENT, GDT_FLAG_4K_GRAN | GDT_FLAG_32_BIT); // Kernel Mode Data segment Descriptor
    gdt_set_descr(&gdt_table_ptr[3], 0, GDT_SEGMENT_LIMIT, GDT_FLAG_RING3 | GDT_FLAG_CODE_DATA_SEGMENT | GDT_FLAG_CODESEG | GDT_FLAG_PRESENT, GDT_FLAG_4K_GRAN | GDT_FLAG_32_BIT); // User Mode Code Segment Descriptor
    gdt_set_descr(&gdt_table_ptr[4], 0, GDT_SEGMENT_LIMIT, GDT_FLAG_RING3 | GDT_FLAG_CODE_DATA_SEGMENT | GDT_FLAG_DATASEG | GDT_FLAG_PRESENT, GDT_FLAG_4K_GRAN | GDT_FLAG_32_BIT); // User Mode Data Segment Descriptor

    gdt_set_descr(&gdt_table_ptr[5], (uint32_t) initial_tss, sizeof(*initial_tss) - 1, GDT_FLAG_ACCESSED | GDT_FLAG_EXEC | GDT_FLAG_PRESENT, GDT_FLAG_32_BIT);	/* TSS */

    /* Flush out the old GDT and install the new changes! */
//	__gdt_flush((uint32_t)&gdt_ptr);
}



/* Setup a descriptor in the Global Descriptor Table */
/*static */void gdt_set_descr(struct gdt_desc_struct *gdt_descr, uint32_t base, uint32_t limit_20, uint8_t access, uint8_t flags_4) {
	gdt_descr->base_low_16 = (base & 0xFFFF);
	gdt_descr->base_middle_8 = (base >> 16) & 0xFF;
	gdt_descr->base_high_8 = (uint8_t) ((base >> 24) & 0xFF);

	gdt_descr->limit_low_16 = (limit_20 & 0xFFFF);
	gdt_descr->limit_high_4 = (limit_20 >> 16) & 0xF;

	gdt_descr->flags = (flags_4 >> 4) & 0xF;
	gdt_descr->access = access;


//struct gdt_ptr_struct gdt_ptr = {.base = &gdt, .limit = 8*3};

//	gdt_ptr.limit = (sizeof(gdt_desc_t) * GDT_ENTRIES) - 1;
//	gdt_ptr.base = (size_t)&gdt;
}
