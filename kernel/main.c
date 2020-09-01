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

#include <arch/x86/multiboot.h>     // for multiboot_info_t


/*
 * Note that linker symbols are not variables; they have no memory allocated
 * for maintaining a value, rather their address is their value. We have to use
 * the address (&) operator to get their address or location. For that, we
 * declare those symbols as char arrays and access them by its name, what is
 * equivalent to do &symbol[0]. (see "linker.ld.pp" file)
 */
IMPORT const char _start[];
IMPORT const char kernel_start[];
IMPORT const char kernel_end[];
IMPORT const char etext[];
IMPORT const char edata[];

struct required_multiboot_info brunix_multiboot_info;

IMPORT void console_init(void);          // from kernel/console.c

// These two lines are here only for the purpose of demonstrating
// ELF executable's sections such as TEXT, DATA and BSS.
PRIVATE int unused_initialized_variable = 5;
PRIVATE int unused_uninitialized_variable;


INIT_FUNC int kmain(multiboot_info_t *mboot_info_ptr, uint32_t magic, uint32_t stack_top);
PRIVATE void print_kernel_context_info(uint32_t total_memory_kb, uint32_t stack_top);


/**
 * This is the main kernel function.
 *
 * @param mboot_info_ptr Contextual information given by the bootloader.
 * @param magic Number representing the Multiboot bootloader magic number.
 * @see multiboot_entry_point.S file
 */
int kmain(multiboot_info_t *mboot_info_ptr, uint32_t magic, uint32_t stack_top) {
    console_init();

    printk("Starting Brunix...\n\n");

    verify_loader(magic);

    save_multiboot_info(mboot_info_ptr, &brunix_multiboot_info);

    if (strnlen(brunix_multiboot_info.cmdline, 43) > 0) {
        printk("Invoking kernel with args: %s\n", brunix_multiboot_info.cmdline);
    }

    print_kernel_context_info(brunix_multiboot_info.mem_upper, stack_top);

    panic("Forcing kernel panic...");       // panic() DOES NOT return!
    return 0;
}

PRIVATE void print_kernel_context_info(uint32_t total_memory_kb, uint32_t stack_top) {
    printk("RAM memory: %u MiB, kernel size: %d KiB\n", roundup_binary(total_memory_kb) / 1024, (kernel_end - kernel_start) / 1024);

    printk("Kernel bootstrap stack: %p\n", stack_top);

    printk("\nSpecial kernel symbols:\n");
    printk("  _start  %08x (phys)\n", _start);
    printk("  text    %08x (virt)  %08x (phys)\n", kernel_start, kernel_start);
    printk("  etext   %08x (virt)  %08x (phys)\n", etext, etext);
    printk("  edata   %08x (virt)  %08x (phys)\n", edata, edata);
    printk("  end     %08x (virt)  %08x (phys)\n", kernel_end, kernel_end);

    printk("\nSome symbol addresses by section:\n");
    printk("  kmain()\t\t\t-> %p (text)\n", &kmain);
    printk("  unused_initialized_variable\t-> %p (data)\n", &unused_initialized_variable);
    printk("  unused_uninitialized_variable\t-> %p (bss)\n", &unused_uninitialized_variable);
}