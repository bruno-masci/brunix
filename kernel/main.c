/**
 * @author Bruno Masci
 * @brief Defines the entry point to C kernel
 *
 * This file contains the kernel's main program.
 */


#include <stdint.h>
#include "../include/brunix/stdio.h"
#include "../include/brunix/stdlib.h"
#include "../include/arch/x86/processor.h"
#include "../include/arch/x86/multiboot.h"
#include "../include/arch/x86/memlayout.h"
#include "../include/arch/x86/uart.h"


/*
 * Note that linker symbols are not variables, they have no memory allocated for
 * maintaining a value, rather their address is their value. So, we have to use
 * the address (&) operator to get their address or location.
 */
extern const uint32_t kernel_start;
extern const uint32_t kernel_end;
extern const uint32_t _start;
extern const uint32_t etext;
extern const uint32_t edata;


static uint32_t initial_esp;


static void verify_loader(uint32_t magic);
static void print_kernel_context_info(multiboot_info_t *mboot_info_ptr);


int kmain(multiboot_info_t *mboot_info_ptr, uint32_t magic, uint32_t initial_stack) {

    initial_esp = initial_stack;

//	video_init();
    cprintf("Starting Brunix...\n\n");

    verify_loader(magic);
    
    uart_init();    // We send all the output to both the screen and the serial port (emulator)


    print_kernel_context_info(mboot_info_ptr);

/*    cprintf("Type 'help' for a list of commands.\n");

    while (1) {
        buf = readline("K> ");
        if (buf != NULL)
            if (runcmd(buf, tf) < 0)
                break;
    }
*/
	//panic_noargs("Nada mejor que hacer!");

	return 0;	// We need to return something...
}

static void verify_loader(uint32_t magic) {
	if (magic != MULTIBOOT_HEADER_MAGIC) {
		panic_noargs("Invalid magic number! A Multiboot compatible loader is needed...");
	}
}

static void print_kernel_context_info(multiboot_info_t *mboot_info_ptr) {

    info("main", "Loading kernel with command line: %s", (char *)mboot_info_ptr->cmdline);
    info("main", "Total RAM installed: %u MB", roundup_binary(mboot_info_ptr->mem_upper) / 1024);
    debug("main", "Kernel size: %d KB; Bootstrap ESP: %x KB", (&kernel_end - &kernel_start) / 1024, initial_esp);
    cprintf("\nSpecial kernel symbols:\n");
    cprintf("  _start                  %p (phys)\n", &_start);    // %08k
    cprintf("  entry  %p (virt)  %p (phys)\n", &kernel_start, &kernel_start - KERN_BASE);
    cprintf("  etext  %p (virt)  %p (phys)\n", &etext, &etext - KERN_BASE);
    cprintf("  edata  %p (virt)  %p (phys)\n", &edata, &edata - KERN_BASE);
    cprintf("  end    %p (virt)  %p (phys)\n", &kernel_end, &kernel_end - KERN_BASE);
    cprintf("size: %d\n", cprintf("prueba: "));
}