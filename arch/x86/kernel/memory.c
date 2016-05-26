//#include <asm/irq.h>
//#include <asm/idt.h>
//#include <asm/isr.h>
#include <asm/paging.h>
#include <brunix/stdio.h>


extern void load_page_directory(page_dir_t *pd);
extern void enable_paging(void);


static page_dir_t page_directory[1024] __attribute__((aligned(4096)));
static page_table_t first_page_table[1024] __attribute__((aligned(4096)));
static page_table_t second_page_table[1024] __attribute__((aligned(4096)));




static void print_page_fault(struct registers_t *regs) {
//	cli();

	unsigned int err_pos;
	asm volatile ("mov %%cr2, %0" : "=r" (err_pos));

	int page_present = regs->err_code & 1;
	int rw = regs->err_code & 2;
	int um = regs->err_code & 4;
	int re = regs->err_code & 8;
	int instruction_fetch = regs->err_code & 16;

	char *instruction_fetch_str = "";
	char * page_present_str = "";
	char *user_mode_str = "";
	char *readwrite = "";
	char *reserved_write = "";	// Overwritten CPU-reserved bits of page entry?
	if(instruction_fetch) instruction_fetch_str = "Fetching instruction";
	if(page_present)    page_present_str = "Page present but protected"; else page_present_str ="No page present";
	if(um)          user_mode_str = "In user mode";
	if(rw)          readwrite = "Writing"; else readwrite = "Reading";
	if(re)          reserved_write = "Reserved write field";

	debug("paging", "Page fault (PF) occurred at %x. PF context: %s - %s - %s - %s - %s", err_pos, page_present_str, readwrite, user_mode_str, reserved_write, instruction_fetch_str);
	debug("paging", "PF extra info => Int. no: %d; CS: %x; EIP: %x; ESP: %x; EFLAGS = %b", regs->int_no, regs->cs, regs->eip, regs->esp, regs->eflags);
}

static void page_fault_callback(struct registers_t *regs) {
	print_page_fault(regs);
	panic_noargs("halting...");
}



void paging_init(void) {
	page_directory[0].present_flag = 1;
	page_directory[0].read_write_flag = 1;
	page_directory[0].user_supervisor_flag = 0;
	page_directory[0].unused_flags = 0;
	page_directory[0].page_table_base_address = TABLE_SHIFT_R(first_page_table);

	for(unsigned int i = 1; i < 1024; i++) {
		page_directory[i].present_flag = 0;
		page_directory[i].read_write_flag = 1;
		page_directory[i].user_supervisor_flag = 0;
		page_directory[i].unused_flags = 0;
	}

	// holds the physical address where we want to start mapping these pages to.
	// in this case, we want to map these pages to the very beginning of memory.
	// We will fill all 1024 entries in the table, mapping 4 megabytes
	for(unsigned int i = 0; i < 1024; i++) {
		// As the address is page aligned, it will always leave 12 bits zeroed.
		// Those bits are used by the attributes ;)
		first_page_table[i].physical_page_address = TABLE_SHIFT_R(i * PAGE_SIZE);
		first_page_table[i].present_flag = 1;
		first_page_table[i].read_write_flag = 1;
		first_page_table[i].user_supervisor_flag = 0;
	}

	printk("page dir= %x --", page_directory);
	printk("first table = %x --", first_page_table);

	register_interrupt_handler(14, &page_fault_callback);

	load_page_directory(page_directory);	// physical address
	enable_paging();
}