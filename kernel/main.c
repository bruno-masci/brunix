/*
 * Copyright (c) 2010, Stefan Lankes, RWTH Aachen University
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *    * Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 *    * Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in the
 *      documentation and/or other materials provided with the distribution.
 *    * Neither the name of the University nor the names of its contributors
 *      may be used to endorse or promote products derived from this
 *      software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

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


uint32_t initial_esp;


static void print_kernel_context_info(multiboot_info_t *mboot_info_ptr);
static void print_timer_ticks();



int kmain(multiboot_info_t *mboot_info_ptr, uint32_t magic, uint32_t initial_stack) {

    initial_esp = initial_stack;

	video_init();

	printk("Starting Brunix...\n\n");

	if (magic != MULTIBOOT2_HEADER_MAGIC) {
		panic_noargs("Invalid magic number! A Multiboot2 compatible loader is needed...");
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

	init_timer(100); // Initialise timer to 100Hz
	print_timer_ticks();

	printk("\nSimulating CPU's exception number 0...\n");
	asm volatile ("int $0x0");

	printk("\nSimulating a syscall...\n");
	asm volatile ("int $0x80");

	print_timer_ticks();

	while(1) {
		HALT;
	}

	return 0;
}

static void print_kernel_context_info(multiboot_info_t *mboot_info_ptr) {
	printk("This is build %u, %u\n", &__BUILD_DATE, &__BUILD_TIME);
	printk("Kernel starts at %x and ends at %x. Total: %u KB\n", &kernel_start, &kernel_end, -(&kernel_end - &kernel_start) / 1024);
	printk("BSS section starts at %x and ends at %x. Total: %d KB\n\n", &bss_start, &bss_end, (&bss_end - &bss_start) / 1024);

	printk("GRUB info...\n");
	printk("Command line: %s\n", (char *)mboot_info_ptr->cmdline);
	printk("Mem lower: %u KB - Mem upper: %u KB\n", mboot_info_ptr->mem_lower, mboot_info_ptr->mem_upper);
	printk("mmap_addr = 0x%x, mmap_length = 0x%x\n\n", (unsigned) mboot_info_ptr->mmap_addr, (unsigned) mboot_info_ptr->mmap_length);// ver https://www.gnu.org/software/grub/manual/multiboot/html_node/kernel_002ec.html
}

static void print_timer_ticks() {
	for (int i=0;i<10000000;i++);	// delay
	debug("kmain", "Clock ticks: %u", get_clock_tick());
}
