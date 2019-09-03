/**
 * @author Bruno Masci
 * @brief Entry point to the C kernel
 *
 * This file contains the kernel's main program.
 */


#include <stdint.h>                 // for uint32_t

#include <brunix/defs.h>            // for PRIVATE, MAX_STR_SIZE?, roundup_binary()
#include <brunix/console.h>
#include <brunix/kernel.h>

#include <arch/x86/multiboot.h>     // for multiboot_info_t


/*
 * Note that linker symbols are not variables; they have no memory allocated
 * for maintaining a value, rather their address is their value. We have to use
 * the address (&) operator to get their address or location. For that, we
 * declare those symbols as char arrays and access them by its name, what is
 * equivalent to do &symbol[0]. (see "linker.ld" file)
 */
extern const char kernel_start[];
extern const char kernel_end[];
extern const char _start[];
extern const char etext[];
extern const char edata[];


void console_init(void);        // from kernel/console.c

// These two lines are here only for the purpose of demonstrating
// ELF executable's sections such as DATA and BSS.
PRIVATE int unused_initialized_variable = 5;
PRIVATE int unused_uninitialized_variable;


PRIVATE void verify_loader(uint32_t magic);
PRIVATE void print_kernel_context_info(multiboot_info_t *mboot_info_ptr);


/**
 * This is the main kernel function.
 *
 * @param mboot_info_ptr Contextual information given by the bootloader.
 * @param magic Number representing the Multiboot bootloader magic number.
 * @see multiboot_entry_point.S file
 */
void kmain(multiboot_info_t *mboot_info_ptr, uint32_t magic) {
    cprintf("Starting Brunix...\n\n");

    verify_loader(magic);

    console_init();

    print_kernel_context_info(mboot_info_ptr);

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
        if (strcmp(mboot_info_ptr->cmdline, "\0") != 0) {
            debug("Loading kernel with command line: %s", (char *)mboot_info_ptr->cmdline);
        }
    }

    debug("Kernel size: %d KB", (kernel_end - kernel_start) / 1024);

    cprintf("\nSpecial kernel symbols:\n");
    cprintf("  _start                  %08x (phys)\n", _start);
    cprintf("  entry  %08x (virt)  %08x (phys)\n", kernel_start, kernel_start);
    cprintf("  etext  %08x (virt)  %08x (phys)\n", etext, etext);
    cprintf("  edata  %08x (virt)  %08x (phys)\n", edata, edata);
    cprintf("  end    %08x (virt)  %08x (phys)\n\n", kernel_end, kernel_end);

    cprintf("\nSome symbol addresses by section:\n");
    cprintf("  kmain()\t\t\t-> %p (text)\n", &kmain);
    cprintf("  unused_initialized_variable\t-> %p (data)\n", &unused_initialized_variable);
    cprintf("  unused_uninitialized_variable\t-> %p (bss)\n", &unused_uninitialized_variable);
}