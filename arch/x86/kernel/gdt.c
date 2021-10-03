#include <asm/gdt.h>
#include <asm/segment.h>
//#include <brunix/stdio.h>


static char *myStack = "pppppppppppppppppppppPPPPPPpppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppp";

// PROTOTYPES
extern void __gdt_flush(uint32_t);
static void gdt_set_desc(int32_t, uint32_t, uint32_t, uint8_t, uint8_t);

static struct tss default_tss = {0, };

struct gdt_desc_struct gdt333[GDT_ENTRIES] = {[0 ... GDT_ENTRIES-1] = {0, 0, 0, 0, 0, 0, 0}};

//        [0] = {0, 0, 0, 0, 0, 0},
//        [1] = {0xFFFF, 0, 0, GDT_FLAG_RING0 | GDT_FLAG_SEGMENT | GDT_FLAG_CODESEG | GDT_FLAG_PRESENT, 0xF, ((GDT_FLAG_4K_GRAN | GDT_FLAG_32_BIT) >> 4) & 0xF},
//        [2] = {0xFFFF, 0, 0, GDT_FLAG_RING0 | GDT_FLAG_SEGMENT | GDT_FLAG_DATASEG | GDT_FLAG_PRESENT, 0xF, ((GDT_FLAG_4K_GRAN | GDT_FLAG_32_BIT) >> 4) & 0xF},
//        [3] = {0xFFFF, 0, 0, GDT_FLAG_RING3 | GDT_FLAG_SEGMENT | GDT_FLAG_CODESEG | GDT_FLAG_PRESENT, 0xF, ((GDT_FLAG_4K_GRAN | GDT_FLAG_32_BIT) >> 4) & 0xF},
//        [4] = {0xFFFF, 0, 0, GDT_FLAG_RING3 | GDT_FLAG_SEGMENT | GDT_FLAG_DATASEG | GDT_FLAG_PRESENT, 0xF, ((GDT_FLAG_4K_GRAN | GDT_FLAG_32_BIT) >> 4) & 0xF},
//        [5] = {0xFFFF, 0, 0, GDT_FLAG_RING3 | GDT_FLAG_SEGMENT | GDT_FLAG_DATASEG | GDT_FLAG_PRESENT, 0xF, ((GDT_FLAG_4K_GRAN | GDT_FLAG_32_BIT) >> 4) & 0xF}
//};
//struct gdt_ptr_struct gdt_ptr = {.base = &gdt, .limit = 8*3};



void gdt_init(void) {
//	gdt_ptr.limit = (sizeof(gdt_desc_t) * GDT_ENTRIES) - 1;
//	gdt_ptr.base = (size_t)&gdt;
printk("AAAAAAAA");
	gdt_set_desc(0, 0, 0, 0, 0);                // Null Segment Descriptor (required)
	gdt_set_desc(1, 0, GDT_SEGMENT_LIMIT, GDT_FLAG_RING0 | GDT_FLAG_CODE_DATA_SEGMENT | GDT_FLAG_CODESEG | GDT_FLAG_PRESENT, GDT_FLAG_4K_GRAN | GDT_FLAG_32_BIT); // Kernel Mode Code Segment Descriptor TODO refactor access flags
	gdt_set_desc(2, 0, GDT_SEGMENT_LIMIT, GDT_FLAG_RING0 | GDT_FLAG_CODE_DATA_SEGMENT | GDT_FLAG_DATASEG | GDT_FLAG_PRESENT, GDT_FLAG_4K_GRAN | GDT_FLAG_32_BIT); // Kernel Mode Data segment Descriptor
	gdt_set_desc(3, 0, GDT_SEGMENT_LIMIT, GDT_FLAG_RING3 | GDT_FLAG_CODE_DATA_SEGMENT | GDT_FLAG_CODESEG | GDT_FLAG_PRESENT, GDT_FLAG_4K_GRAN | GDT_FLAG_32_BIT); // User Mode Code Segment Descriptor
	gdt_set_desc(4, 0, GDT_SEGMENT_LIMIT, GDT_FLAG_RING3 | GDT_FLAG_CODE_DATA_SEGMENT | GDT_FLAG_DATASEG | GDT_FLAG_PRESENT, GDT_FLAG_4K_GRAN | GDT_FLAG_32_BIT); // User Mode Data Segment Descriptor


    default_tss.debug_flag = 0x00;
	default_tss.io_map = 0xFFFF;
	default_tss.esp0 = (((uint32_t) &myStack) + 200);
	default_tss.ss0 = __USER_DS_SELECTOR;
	default_tss.io_map = (uint16_t) sizeof(default_tss);
	gdt_set_desc(5, (uint32_t) &default_tss, sizeof(default_tss) - 1, GDT_FLAG_ACCESSED | GDT_FLAG_EXEC | GDT_FLAG_PRESENT, GDT_FLAG_32_BIT);	/* TSS */

	/* Flush out the old GDT and install the new changes! */
//	__gdt_flush((uint32_t)&gdt_ptr);
printk("SSSSSSS");
}



/* Setup a descriptor in the Global Descriptor Table */
static void gdt_set_desc(int32_t num, uint32_t base, uint32_t limit_20, uint8_t access, uint8_t flags_4) {
	/* Setup the descriptor base address */
	gdt333[num].base_low_16 = (base & 0xFFFF);
	gdt333[num].base_middle_8 = (base >> 16) & 0xFF;
	gdt333[num].base_high_8 = (base >> 24) & 0xFF;

	/* Setup the descriptor limits */
	gdt333[num].limit_low_16 = (limit_20 & 0xFFFF);
	gdt333[num].limit_high_4 = (limit_20 >> 16) & 0xF;

	gdt333[num].flags = (flags_4 >> 4) & 0xF;
    gdt333[num].access = access;

    printk("ACCESS = %x\n", gdt333[num].access);
    printk("FLAGS = %x\n", gdt333[num].flags);
}
