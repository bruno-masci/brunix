/**
 * @author Bruno Masci
 * @brief Entry point to the C kernel
 *
 * This file contains the kernel's main program.
 */


#include <stdint.h>                 // for uint32_t

#include <brunix/defs.h>            // for PRIVATE, MAX_STR_SIZE?, roundup_binary()
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
extern const char _start[];
extern const char kernel_start[];
extern const char kernel_end[];
extern const char etext[];
extern const char edata[];

struct required_multiboot_info brunix_multiboot_info;

void video_init(void);          // from kernel/console.c

// These two lines are here only for the purpose of demonstrating
// ELF executable's sections such as TEXT, DATA and BSS.
PRIVATE int unused_initialized_variable = 5;
PRIVATE int unused_uninitialized_variable;


INIT int kmain(multiboot_info_t *mboot_info_ptr, uint32_t magic, uint32_t stack_top);
PRIVATE void print_kernel_context_info(uint32_t total_memory_kb, uint32_t stack_top);


/**
 * This is the main kernel function.
 *
 * @param mboot_info_ptr Contextual information given by the bootloader.
 * @param magic Number representing the Multiboot bootloader magic number.
 * @see multiboot_entry_point.S file
 */
int kmain(multiboot_info_t *mboot_info_ptr, uint32_t magic, uint32_t stack_top) {
    video_init();

    cprintf("Starting Brunix...\n\n");

    verify_loader(magic);

    save_multiboot_info(mboot_info_ptr, &brunix_multiboot_info);

    if (strnlen(brunix_multiboot_info.cmdline, 43) > 0) {
        cprintf("Invoking kernel with args: %s\n", brunix_multiboot_info.cmdline);
    }

    print_kernel_context_info(brunix_multiboot_info.mem_upper, stack_top);

    panic("Forcing kernel panic...");       // panic() DOES NOT return!
}

PRIVATE void print_kernel_context_info(uint32_t total_memory_kb, uint32_t stack_top) {
    cprintf("RAM memory: %u MiB, kernel size: %d KiB\n", roundup_binary(total_memory_kb) / 1024, (kernel_end - kernel_start) / 1024);

    cprintf("Kernel bootstrap stack: %p\n", stack_top);

    cprintf("\nSpecial kernel symbols:\n");
    cprintf("  _start  %08x (phys)\n", _start);
    cprintf("  text    %08x (virt)  %08x (phys)\n", kernel_start, kernel_start);
    cprintf("  etext   %08x (virt)  %08x (phys)\n", etext, etext);
    cprintf("  edata   %08x (virt)  %08x (phys)\n", edata, edata);
    cprintf("  end     %08x (virt)  %08x (phys)\n", kernel_end, kernel_end);

    cprintf("\nSome symbol addresses by section:\n");
    cprintf("  kmain()\t\t\t-> %p (text)\n", &kmain);
    cprintf("  unused_initialized_variable\t-> %p (data)\n", &unused_initialized_variable);
    cprintf("  unused_uninitialized_variable\t-> %p (bss)\n", &unused_uninitialized_variable);
}