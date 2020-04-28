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

#include "kalloc.h"


/*
 * Note that linker symbols are not variables; they have no memory allocated
 * for maintaining a value, rather their address is their value. We have to use
 * the address (&) operator to get their address or location. For that, we
 * declare those symbols as char arrays and access them by its name, what is
 * equivalent to do &symbol[0]. (see "linker.ld.pp" file)
 */
extern const char kernel_start[];
extern const char kernel_end[];
extern const char etext[];
extern const char edata[];


void console_init(void);        // from kernel/console.c

// These two lines are here only for the purpose of demonstrating
// ELF executable's sections such as TEXT, DATA and BSS.
PRIVATE int unused_initialized_variable = 5;
PRIVATE int unused_uninitialized_variable;


PRIVATE void verify_loader(uint32_t magic);
PRIVATE void print_kernel_context_info(multiboot_info_t *mboot_info_ptr);


__attribute__((__aligned__(PAGE_SIZE)))
pde_t highmem_entrypgdir[NPDENTRIES] = {
        // Map VA's [0, 4MB) to PA's [0, 4MB)
        [0] = (0) | PTE_P | PTE_W | PTE_PS,//FIXME por que no puedo sacar esto????
        // Map VA's [KERNBASE, KERNBASE+4MB) to PA's [0, 4MB)
        [KERN_BASE>>PDXSHIFT] = (0) | PTE_P | PTE_W | PTE_PS,
};

// Boot page table used in kernel/multiboot_entry_point.S.
// Page directories (and page tables), must start on a page boundary,
// hence the "__aligned__" attribute.
// Use PTE_PS in page directory entry to enable 4Mbyte pages.
__attribute__((__aligned__(PAGE_SIZE)))
pde_t entrypgdir[NPDENTRIES] = {
        // Map VA's [0, 4MB) to PA's [0, 4MB)
        [0] = (0) | PTE_P | PTE_W | PTE_PS,
        // Map VA's [KERNBASE, KERNBASE+4MB) to PA's [0, 4MB)
        [KERN_BASE>>PDXSHIFT] = (0) | PTE_P | PTE_W | PTE_PS,
};


/**
 * This is the main kernel function.
 *
 * @param mboot_info_ptr Contextual information given by the bootloader.
 * @param magic Number representing the Multiboot bootloader magic number.
 * @see multiboot_entry_point.S file
 */
void kmain(multiboot_info_t *mboot_info_ptr, uint32_t magic) {

//    entrypgdir[0] = (0) | PTE_P | PTE_W | PTE_PS;
//    asm volatile("movl %0,%%cr3" : : "r" (VIRT_TO_PHYS_WO(&entrypgdir)));

    // In case GRUB doesn't do this...
    // Before doing anything else, complete the ELF loading process.
    // Clear the uninitialized global data (BSS) section of our program.
    // This ensures that all static/global variables start out zero.
    memset(edata, 0, kernel_end - edata);

    cprintf("Starting Brunix...\n\n");

    verify_loader(magic);

    console_init();

    print_kernel_context_info(mboot_info_ptr);

    cprintf("Setting up GDT...\n");
    gdt_init();

    cprintf("Installing new page table %x (%x, %x)...\n", &highmem_entrypgdir, VIRT_TO_PHYS_WO(&highmem_entrypgdir), VIRT_TO_PHYS(&highmem_entrypgdir));
//    asm volatile("movl %0,%%cr3" : : "r" (VIRT_TO_PHYS_WO(&entrypgdir)));
    asm volatile("movl %0,%%cr3" : : "r" (0x00204000)); //FIXME

    cprintf("Initializing physical page allocator...\n");
    kinit1(kernel_end, PHYS_TO_VIRT(MB_TO_BYTES(4))); // phys page allocator


    // test physical memory allocator
    char *mem = kalloc();
    if(mem == 0) {
        panic("Out of memory. This should not happen...\n");
    }
    cprintf("kalloc() returned page address: %p (virt). Zeroing page...\n", mem);
//    if (VIRT_TO_PHYS(mem) != MB_TO_BYTES(4) - PAGE_SIZE) {
//        panic(">> Unexpected returned address by kalloc(): %p", mem);
//    } temporal comment
    for (int i = 0; i < 254; i++) {     // requesting PAGE_SIZE * 256 = 1 MBy of memory
        mem = kalloc();
        if(mem == 0) {
            panic("Out of memory. This should not happen...\n");
        }

        memset(mem, 0, PAGE_SIZE);
    }

    mem = kalloc();
    if(mem == 0) {
        panic("Out of memory. This should not happen...\n");
    }
    cprintf("kalloc() returned page address: %p (virt). Zeroing page...\n", mem);
//    if (VIRT_TO_PHYS(mem) != MB_TO_BYTES(4) - MB_TO_BYTES(1)) {
//        panic("Unexpected returned address by kalloc(): %p", mem);
//    }temporal comment TODO


//    kvmalloc();      // kernel page table

    panic("Forcing kernel panic...");       // panic() DOES NOT return!
}

PRIVATE void verify_loader(uint32_t magic) {
    if (magic != MBOOT_LOADER_MAGIC) {
        panic("Invalid magic number! A Multiboot compatible loader is needed...");
    }
}

PRIVATE void print_kernel_context_info(multiboot_info_t *mboot_info_ptr) {
    info("Total RAM installed: %u MB", roundup_binary(mboot_info_ptr->mem_upper) / 1024);

    /* TODO mover esto!! Check if the bit BIT in FLAGS is set. */
#define CHECK_FLAG(flags,bit)   ((flags) & (1 << (bit)))

    /* is the command-line defined? */
#define MULTIBOOT_INFO_CMDLINE                  0x00000004

    if (CHECK_FLAG(mboot_info_ptr->flags, 2)) {
        if (strcmp(((char *) mboot_info_ptr->cmdline), "\0") != 0) {
            debug("Loading kernel with command line: %s", (char *)mboot_info_ptr->cmdline);
        }
    }

    debug("Kernel size: %d KB", (kernel_end - kernel_start) / 1024);

    cprintf("\nSpecial kernel symbols:\n");
    cprintf("  _start  %08x (virt)  %08x (phys)\n", kernel_start, VIRT_TO_PHYS_WO(kernel_start));
    cprintf("  etext   %08x (virt)  %08x (phys)\n", etext, VIRT_TO_PHYS_WO(etext));
    cprintf("  edata   %08x (virt)  %08x (phys)\n", edata, VIRT_TO_PHYS(edata));
    cprintf("  end     %08x (virt)  %08x (phys)\n\n", kernel_end, kernel_end);

    cprintf("\nSome symbol addresses by section:\n");
    cprintf("  kmain()\t\t\t-> %p (text)\n", &kmain);
    cprintf("  unused_initialized_variable\t-> %p (data)\n", &unused_initialized_variable);
    cprintf("  unused_uninitialized_variable\t-> %p (bss)\n", &unused_uninitialized_variable);
}

