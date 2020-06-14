/**
 * @author Bruno Masci
 * @brief Entry point to the C kernel
 *
 * This file contains the kernel's main program.
 */


#include <stdint.h>                 // for uint32_t

#include <brunix/defs.h>            // for PRIVATE, MAX_STR_SIZE?, roundup_binary(), MB_TO_BYTES
#include <brunix/string.h>
#include <brunix/console.h>
#include <brunix/kernel.h>

#include <arch/x86/multiboot.h>     // for multiboot_info_t
#include <arch/x86/memlayout.h>     // for PHYS_TO_VIRT
#include <arch/x86/mmu.h>
#include <arch/x86/types.h>
#include <arch/x86/gdt.h>   //TODO
#include <arch/x86/irq.h>   //TODO
#include <arch/x86/timer.h>   //TODO
#include <arch/x86/paging.h>   //TODO

#include "kalloc.h"


/*
 * Note that linker symbols are not variables; they have no memory allocated
 * for maintaining a value, rather their address is their value. We have to use
 * the address (&) operator to get their address or location. For that, we
 * declare those symbols as char arrays and access them by its name, what is
 * equivalent to do &symbol[0]. (see "linker.ld.pp" file)
 */
extern const char _start[];
extern const char kernel_start[];
extern const char kernel_end[];
extern const char etext[];
extern const char edata[];

extern void kbd_init(void);
extern void idt_flush(void);

struct required_multiboot_info brunix_multiboot_info;

void console_init(void);        // from kernel/console.c

// These two lines are here only for the purpose of demonstrating
// ELF executable's sections such as TEXT, DATA and BSS.
PRIVATE int unused_initialized_variable = 5;
PRIVATE int unused_uninitialized_variable;


PRIVATE void print_kernel_context_info(uint32_t total_memory_kb);
PRIVATE void print_segment_selectors(void);



void handle_command(char *cmd) {
//    cprintf("Received command: %s\n", cmd);
    if (!strcmp("selectors", cmd)) {
        print_segment_selectors();
    } else if (!strcmp("info", cmd)) {
        print_kernel_context_info(brunix_multiboot_info.mem_upper);
    } else if (!strcmp("ticks", cmd)) {
        cprintf("%d\n", get_clock_ticks());
    } else if (!strcmp("bootargs", cmd)) {
        cprintf("%s\n", brunix_multiboot_info.cmdline);
    } else if (!strcmp("dani...", cmd)) {
        cprintf("%s\n", "te amo!! :)");
    }
}

// Boot page table used in kernel/multiboot_entry_point.S.
// Page directories (and page tables), must start on a page boundary,
// hence the "__aligned__" attribute.
// Use PTE_PS in page directory entry to enable 4Mbyte pages.
__attribute__((__aligned__(PAGE_SIZE)))
pde_t entrypgdir[NPDENTRIES] = {
        // Map VA's [0, 4MB) to PA's [0, 4MB)
        [0] = (0) | PTE_P | PTE_W | PTE_PS,
        // Map VA's [KERNBASE, KERNBASE+4MB) to PA's [0, 4MB)
//        [KERN_BASE>>PDXSHIFT] = (0) | PTE_P | PTE_W | PTE_PS,
};

#pragma GCC diagnostic ignored "-Wpedantic" //TODO remove
page_dir_t newentrypgdir = {
    present_flag: true,
    read_write_flag: true,
    user_supervisor_flag: true
//    page_table_base_address
};

/**
 * This is the main kernel function.
 *
 * @param mboot_info_ptr Contextual information given by the bootloader.
 * @param magic Number representing the Multiboot bootloader magic number.
 * @see multiboot_entry_point.S file
 */
int kmain(multiboot_info_t *mboot_info_ptr, uint32_t magic, uint32_t stack_top) {
    // In case GRUB doesn't do this...
    // Before doing anything else, complete the ELF loading process.
    // Clear the uninitialized global data (BSS) section of our program.
    // This ensures that all static/global variables start out zero.
//    memset(edata, 0, kernel_end - edata);//TODO check this

//    verify_loader(magic);
    struct required_multiboot_info brunix_multiboot_info2;
//    save_multiboot_info(mboot_info_ptr, &brunix_multiboot_info2);
    save_multiboot_info(mboot_info_ptr, &brunix_multiboot_info);

    cprintf("MEMUPPER BRUNIX: %d\n", brunix_multiboot_info.mem_upper);

    cprintf("Starting Brunix...\n\n");
    debug("Kernel bootstrap stack: %p", stack_top);
    extern uint32_t total_ram_memory;
    cprintf("Total RAM memory: %d\n", total_ram_memory);

    cprintf("Setting up GDT...\n");
    gdt_init();

    console_init();

    print_kernel_context_info(brunix_multiboot_info.mem_upper);

//    cprintf("IRQs...\n");
    irq_init();
    timer_init(30); // Initialise timer to 100Hz FIXME

    cprintf("Keyboard...");
    kbd_init();
    idt_flush();

    debug_noargs("Enabling interrupts...");
    asm volatile("sti");


//    int f=1/0;


//    cprintf("entrypgdir address: %p...\n", VIRT_TO_PHYS_WO(entrypgdir));
//    uint32_t addr = VIRT_TO_PHYS(&entrypgdir2);  //FIXME con esta anda; con _WO no!!
//    cprintf("addr address: %p...\n", addr);
//    __load_page_directory(addr);
//    __enable_paging();

//	cprintf("\nSimulating CPU's exception number 0...\n");
//	asm volatile ("int $0x0");

//	cprintf("\nSimulating a syscall...\n");
//	asm volatile ("int $0x80");

//    cprintf("\nGenerating a page fault...");
//    uint32_t *ptr = (uint32_t *)0x400000;
//    uint32_t do_page_fault = *ptr;

    //FIXME panic("Forcing kernel panic...");       // panic() DOES NOT return!
    cprintf(">> ");
    for(;;);
}

PRIVATE void print_segment_selectors(void) {
    uint32_t cs;
    uint32_t ds;
    uint32_t es;
    uint32_t ss;
    asm("movl %%cs, %0" : "=r" (cs) ::);
    asm("movl %%ds, %0" : "=r" (ds) ::);
    asm("movl %%es, %0" : "=r" (es) ::);
    asm("movl %%ss, %0" : "=r" (ss) ::);
    cprintf("CS 0x%x, DS 0x%x, ES 0x%x, SS 0x%x\n", cs, ds, es, ss);
}

PRIVATE void print_kernel_context_info(uint32_t total_memory_kb) {
    info("Total RAM installed: %u MiB", roundup_binary(total_memory_kb) / 1024);

    debug("Kernel size: %d KB", (kernel_end - kernel_start) / 1024);

    cprintf("\nSpecial kernel symbols:\n");
    cprintf("  _start  %08x (phys)\n", _start);
    cprintf("  text    %08x (virt)  %08x (phys)\n", kernel_start, VIRT_TO_PHYS_WO(kernel_start));
    cprintf("  etext   %08x (virt)  %08x (phys)\n", etext, VIRT_TO_PHYS_WO(etext));
    cprintf("  edata   %08x (virt)  %08x (phys)\n", edata, VIRT_TO_PHYS(edata));
    cprintf("  end     %08x (virt)  %08x (phys)\n\n", kernel_end, kernel_end);

    cprintf("\nSome symbol addresses by section:\n");
    cprintf("  kmain()\t\t\t-> %p (text)\n", &kmain);
    cprintf("  unused_initialized_variable\t-> %p (data)\n", &unused_initialized_variable);
    cprintf("  unused_uninitialized_variable\t-> %p (bss)\n", &unused_uninitialized_variable);
}