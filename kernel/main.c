/**
 * @author Bruno Masci
 * @brief Defines the entry point to C kernel
 *
 * This file contains the kernel's main program.
 */


#include <stdint.h>
#include "../include/brunix/stdio.h"
#include "../include/arch/x86/processor.h"
#include "../include/arch/x86/multiboot.h"


/*
 * Note that linker symbols are not variables, they have no memory allocated for
 * maintaining a value, rather their address is their value.
 */
extern const void kernel_start;
extern const void kernel_end;


static uint32_t initial_esp;


static void print_kernel_context_info(multiboot_info_t *mboot_info_ptr);


int kmain(multiboot_info_t *mboot_info_ptr, uint32_t magic, uint32_t initial_stack) {

    initial_esp = initial_stack;

	video_init();

	printk("Starting Brunix...\n\n");

	if (magic != MULTIBOOT_HEADER_MAGIC) {
		panic_noargs("Invalid magic number! A Multiboot compatible loader is needed...");
    }

	print_kernel_context_info(mboot_info_ptr);

	panic_noargs("Nada mejor que hacer!");

	return 0;
}

static void print_kernel_context_info(multiboot_info_t *mboot_info_ptr) {
    printk("Total RAM installed: %u MB\n", mboot_info_ptr->mem_upper / 1024);
    printk("Loading kernel with command line: %s\n", (char *)mboot_info_ptr->cmdline);
    debug("main", "Kernel size: %d KB; Bootstrap ESP: %x KB", (&kernel_end - &kernel_start) / 1024, initial_esp);
}