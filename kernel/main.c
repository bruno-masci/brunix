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
#include <brunix/kdebug.h>

#include <arch/x86/multiboot.h>     // for multiboot_info_t
#include <arch/x86/memlayout.h>     // for PHYS_TO_VIRT


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

//struct required_multiboot_info brunix_multiboot_info;

void console_init(void);        // from kernel/console.c

// These two lines are here only for the purpose of demonstrating
// ELF executable's sections such as TEXT, DATA and BSS.
PRIVATE int unused_initialized_variable = 5;
PRIVATE int unused_uninitialized_variable;


PRIVATE void print_kernel_context_info(uint32_t total_memory_kb);
PRIVATE void print_segment_selectors(void);

void recur1(int i) {
    if (i==0) return;
    stack_backtrace();
    recur1(--i);
}

/**
 * This is the main kernel function.
 *
 * @param mboot_info_ptr Contextual information given by the bootloader.
 * @param magic Number representing the Multiboot bootloader magic number.
 * @see multiboot_entry_point.S file
 */
int kmain(multiboot_info_t *mboot_info_ptr, uint32_t magic, uint32_t stack_top) {
    stack_backtrace();
//    cprintf("EBP %x\n", read_ebp());
//    cprintf("ADDR %x\n", &stack_backtrace);

//    verify_loader(magic);

//    print_segment_selectors();

//    char a[264] = "\0";
//    struct required_multiboot_info brunix_multiboot_info2;
//    save_multiboot_info(mboot_info_ptr, &brunix_multiboot_info);

//    cprintf("Total RAM memory: %d\n", brunix_multiboot_info.mem_upper);

    cprintf("Starting Brunix...\n\n");
    debug("Kernel bootstrap stack: %p", stack_top);

    console_init();

//    stack_backtrace();
    recur1(3);

    cprintf("6828 decimal is %o octal!\n", 6828);

    //    cprintf("mem upppperrrrrrrrrrrrrrrrrrrr: %d\n", brunix_multiboot_info.mem_upper);
//    print_kernel_context_info(brunix_multiboot_info.mem_upper);
    print_kernel_context_info(10);

    panic("Forcing kernel panic...");       // panic() DOES NOT return!
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
    cprintf("  text    %08x (virt)  %08x (phys)\n", kernel_start, kernel_start);
    cprintf("  etext   %08x (virt)  %08x (phys)\n", etext, etext);
    cprintf("  edata   %08x (virt)  %08x (phys)\n", edata, edata);
    cprintf("  end     %08x (virt)  %08x (phys)\n\n", kernel_end, kernel_end);

//    cprintf("\nSome symbol addresses by section:\n");
//    cprintf("  kmain()\t\t\t-> %p (text)\n", &kmain);
//    cprintf("  unused_initialized_variable\t-> %p (data)\n", &unused_initialized_variable);
//    cprintf("  unused_uninitialized_variable\t-> %p (bss)\n", &unused_uninitialized_variable);
}