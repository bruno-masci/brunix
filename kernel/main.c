/**
 * @author Bruno Masci
 * @brief Defines the entry point to C kernel
 *
 * This file contains the kernel's main program.
 */


#include <asm/gdt.h>
#include <asm/idt.h>
#include <asm/irq.h>
#include "timer.h"
#include <brunix/stdio.h>
#include <brunix/processor.h>
#include <asm/multiboot.h>



/*
 * Note that linker symbols are not variables, they have no memory allocated for
 * maintaining a value, rather their address is their value.
 */
extern const void kernel_start;
extern const void kernel_end;
extern const void bss_start;
extern const void bss_end;
extern char __BUILD_DATE;
extern char __BUILD_TIME;


static uint32_t initial_esp;


static void print_kernel_context_info(multiboot_info_t *mboot_info_ptr);
static void print_timer_ticks();



int kmain(multiboot_info_t *mboot_info_ptr, uint32_t magic, uint32_t initial_stack) {

    initial_esp = initial_stack;

	video_init();

	printk("Starting Brunix...\n\n");

	if (magic != MULTIBOOT_HEADER_MAGIC) {
		panic_noargs("Invalid magic number! A Multiboot compatible loader is needed...");
    }

	// TODO initialize .bss section (de eduOS)
	//memset((void*)&bss_start, 0x00, ((size_t) &bss_end - (size_t) &bss_start));


	print_kernel_context_info(mboot_info_ptr);

	printk("Initializing GDT...\n");
	gdt_init();
	printk("Initializing IRQs...\n");
	irq_init();


	debug_noargs("kmain", "Enabling interrupts...");
	asm volatile("sti");

	timer_init(100); // Initialise timer to 100Hz
	print_timer_ticks();

	printk("Installing keyboard...");
	kbd_init();


//	io.print("Loading Task Register \n");
//	asm("	movw $0x38, %ax; ltr %ax"); 38 === descriptor en gdt

	//printk("\nSimulating CPU's exception number 0...\n");
//	asm volatile ("int $0x0");

//	printk("\nSimulating a syscall...\n");
//	asm volatile ("int $0x80");

	while(1) {
		HALT;
	}

	return 0;
}

static void print_kernel_context_info(multiboot_info_t *mboot_info_ptr) {
	printk("-- This is build %u, %u --\n", &__BUILD_DATE, &__BUILD_TIME);
	printk("Kernel size: %d KB. Initial ESP: %x KB\n", (&kernel_end - &kernel_start) / 1024, initial_esp);
	printk("BSS section starts at %x and ends at %x. Total: %d KB\n\n", &bss_start, &bss_end, (&bss_end - &bss_start) / 1024);

	// VER https://www.gnu.org/software/grub/manual/multiboot/html_node/kernel_002ec.html
	printk("OS booted with %s...\n", (char *)mboot_info_ptr->boot_loader_name);
	printk("  -> Command line: %s\n", (char *)mboot_info_ptr->cmdline);
	printk("  -> Mem lower: %u KB - Mem upper: %u MB\n", mboot_info_ptr->mem_lower, mboot_info_ptr->mem_upper / 1024);
	printk("  -> TODO (http://forum.osdev.org/viewtopic.php?f=1&t=10934&start=0) mmap_addr = 0x%x, mmap_length = 0x%x\n\n", (unsigned) mboot_info_ptr->mmap_addr, (unsigned) mboot_info_ptr->mmap_length);// ver https://www.gnu.org/software/grub/manual/multiboot/html_node/kernel_002ec.html
}

static void print_timer_ticks() {
	for (int i=0;i<10000000;i++);	// delay
	debug("kmain", "Clock ticks: %u", get_clock_tick());
}
