/**
 * @author Bruno Masci
 * @brief Entry point to the C kernel
 *
 * This file contains the kernel's main program.
 */


#include <stdint.h>                 // for uint32_t

#include <brunix/defs.h>            // for PRIVATE, IMPORT, INIT_FUNC, MAX_STR_SIZE?, roundup_binary()
#include <brunix/string.h>
#include <brunix/console.h>
#include <brunix/kernel.h>
#include <brunix/kdebug.h>
#include <arch/x86/gdt.h>

#include <arch/x86/multiboot.h>     // for struct std_multiboot_info, struct multiboot_info

#include <arch/x86/memlayout.h>     // for VIRT_TO_PHYS_WO


///
#include <arch/x86/mmu.h>
#include <arch/x86/paging.h>   //TODO

/*
 * Note that linker symbols are not variables; they have no memory allocated
 * for maintaining a value, rather their address is their value. We have to use
 * the address (&) operator to get their address or location. For that, we
 * declare those symbols as char arrays and access them by its name, what is
 * equivalent to do &symbol[0]. (see "linker.ld.pp" file)
 */
IMPORT const char kernel_start[];
IMPORT const char kernel_end[];

struct multiboot_info mboot_info;

IMPORT void console_init(void);          // from kernel/console.c


INIT_FUNC int kmain(struct std_multiboot_info *std_mboot_info, uint32_t magic, uint32_t stack_top);
PRIVATE void print_kernel_context_info(uint32_t total_memory_kb, uint32_t stack_top);

PRIVATE void print_segment_selectors(void);
PRIVATE void process_boot_args(const char *boot_args);



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


/**
 * This is the main kernel function.
 *
 * @param std_mboot_info Contextual information given by the bootloader.
 * @param magic Number representing the Multiboot bootloader magic number.
 * @see multiboot_entry_point.S file
 */
int kmain(struct std_multiboot_info *std_mboot_info, uint32_t magic, uint32_t stack_top) {
    console_init();

    printk("Starting Brunix...\n\n");

    verify_loader(magic);

    printk("Segment selectors (bootloader) -> ");
    print_segment_selectors();

    printk("Setting up GDT...\n");
    gdt_init();

    printk("Segment selectors (brunix) -> ");
    print_segment_selectors();
    printk("\n");

    save_multiboot_info(std_mboot_info, &mboot_info);

    if (strnlen(mboot_info.cmdline, 43) > 0) {
        printk("Invoking kernel with args: %s\n", mboot_info.cmdline);
        process_boot_args(mboot_info.cmdline);
    }

    printk("entrypgdir address: %p...\n", VIRT_TO_PHYS_WO(entrypgdir));
    page_dir_t *addr = (page_dir_t *) VIRT_TO_PHYS_WO(&entrypgdir);  //FIXME con esta anda; con _WO no!!
    printk("addr address: %p...\n", addr);
    load_page_directory(&entrypgdir);
    enable_paging();

    print_kernel_context_info(mboot_info.mem_upper, stack_top);

    panic("Forcing kernel panic...");       // panic() DOES NOT return!
    return 0;
}

PRIVATE void process_boot_args(const char *boot_args) {
    //TODO
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
    printk("CS 0x%x, DS 0x%x, ES 0x%x, SS 0x%x\n", cs, ds, es, ss);
}

PRIVATE void print_kernel_context_info(uint32_t total_memory_kb, uint32_t stack_top) {
    printk("RAM memory: %u MiB, kernel size: %d KiB\n", roundup_binary(total_memory_kb) / 1024, (kernel_end - kernel_start) / 1024);

    printk("Kernel bootstrap stack: %p\n", stack_top);

    printk("\nSome special kernel symbols:\n");
    printk("  text    %08x (virt)  %08x (phys)\n", kernel_start, VIRT_TO_PHYS_WO(kernel_start));
    printk("  end     %08x (virt)  %08x (phys)\n\n", kernel_end, VIRT_TO_PHYS(kernel_end));
}
