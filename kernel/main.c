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
#include <asm/gdt.h>

#include <asm/multiboot.h>     // for struct std_multiboot_info, struct multiboot_info

#include <asm/memlayout.h>     // for VIRT_TO_PHYS_WO


///
#include <asm/mmu.h>
#include <asm/paging.h>   //TODO

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


INIT_FUNC void gdt_init(void);
INIT_FUNC int kmain(struct std_multiboot_info *std_mboot_info, uint32_t magic, uint32_t stack_top);
PRIVATE void print_kernel_context_info(uint32_t total_memory_kb, uint32_t stack_top);

PRIVATE void process_boot_args(struct multiboot_info mb_info);

PRIVATE void exercise_libkern(void);


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

//    exercise_libkern();

    verify_loader(magic);

    save_multiboot_info(std_mboot_info, &mboot_info);

    process_boot_args(mboot_info);

    print_kernel_context_info(mboot_info.mem_upper, stack_top);

    panic("Forcing kernel panic...");       // panic() DOES NOT return!
    return 0;
}

PRIVATE void process_boot_args(struct multiboot_info mb_info) {
    if (strnlen(mb_info.cmdline, 43) > 0) {
        printk("Invoking kernel with args: %s\n", mb_info.cmdline);
    }
}

PRIVATE void print_kernel_context_info(uint32_t total_memory_kb, uint32_t stack_top) {
    printk("RAM memory: %u MiB, kernel size: %d KiB\n", roundup_binary(total_memory_kb) / 1024, (kernel_end - kernel_start) / 1024);

    printk("Kernel bootstrap stack: %p; kmain(): %p\n", stack_top, kmain);

    printk("\nSome special kernel symbols:\n");
    printk("  text    %08x (virt)  %08x (phys)\n", kernel_start, VIRT_TO_PHYS_WO(kernel_start));
    printk("  end     %08x (virt)  %08x (phys)\n\n", kernel_end, VIRT_TO_PHYS(kernel_end));
}

PRIVATE void exercise_libkern(void) {
    printk("6828 decimal is %o octal!\n", 6828);
    printk("131 decimal is %b binary!\n", 131);
    printk("132 decimal is %d signed integer!\n", 132);
    printk("-133 decimal is %d signed integer!\n", -133);
    printk("134 decimal is %u unsigned integer!\n", 134);
    printk("Null string is %s\n", NULL);
    printk("String brunix is %s\n", "brunix");
}