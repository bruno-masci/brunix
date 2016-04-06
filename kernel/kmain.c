#include <asm/gdt.h>
#include <asm/idt.h>
#include <asm/irq.h>
#include <asm/vga.h>
#include "timer.h"
#include <brunix/printk.h>
#include "multiboot.h"



uint32_t initial_esp;




extern const void kernel_start;
extern const void kernel_end;
extern const void bss_start;
extern const void bss_end;
extern char __BUILD_DATE;
extern char __BUILD_TIME;



void print_kernel_context_info(multiboot_info_t *mboot_info_ptr) {
	printk("# %s\n\n", (char *)mboot_info_ptr->cmdline);

	printk("Kernel starts at %x and ends at %x. Total: %d KB\n", &kernel_start, &kernel_end, -(&kernel_end - &kernel_start) / 1024);
	printk("BSS section starts at %x and ends at %x. Total: %d KB\n", &bss_start, &bss_end, (&bss_end - &bss_start) / 1024);
	printk("Mem lower: %u KB - Mem upper: %u KB\n", mboot_info_ptr->mem_lower, mboot_info_ptr->mem_upper);
	printk("mmap_addr = 0x%x, mmap_length = 0x%x\n\n", (unsigned) mboot_info_ptr->mmap_addr, (unsigned) mboot_info_ptr->mmap_length);// ver https://www.gnu.org/software/grub/manual/multiboot/html_node/kernel_002ec.html
	printk("Build date: %d, time: %d\n\n", &__BUILD_DATE, &__BUILD_TIME);
}

void print_timer_ticks() {
	for (int i=0;i<10000000;i++);	// delay
	debug("kmain", "Clock ticks: %u", get_clock_tick());
}


int kmain(multiboot_info_t *mboot_info_ptr, uint32_t magic, uint32_t initial_stack) {

    initial_esp = initial_stack;

	vga_init();

	printk("Starting Brunix...\n\n");

	if (magic != MULTIBOOT2_HEADER_MAGIC) {
		panic_noargs("Invalid magic number! A Multiboot2 compatible loader is needed...");
    }

	print_kernel_context_info(mboot_info_ptr);

	printk("Initializing GDT...\n");
	gdt_init();
	printk("Initializing IRQs...\n");
	irq_init();

 // TODO initialize .bss section (de eduOS)
        //memset((void*)&bss_start, 0x00, ((size_t) &bss_end - (size_t) &bss_start));



	debug_noargs("kmain", "Enabling interrupts...");
	asm volatile("sti");

	init_timer(100); // Initialise timer to 100Hz
	print_timer_ticks();

	printk("\nSimulating CPU's exception number 0...\n");
	asm volatile ("int $0x0");

	printk("\nSimulating a syscall...\n");
	asm volatile ("int $0x80");

	print_timer_ticks();

	return 0;
}
