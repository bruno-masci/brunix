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

#include <asm/irq.h>   //TODO
#include <asm/timer.h>   //TODO
#include <asm/paging.h>   //TODO
//#include <asm/irq.h>

#include "brunix/mm.h"

///
#include <asm/mmu.h>
#include <asm/page.h>   //TODO
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

extern void kbd_init(void);
extern void idt_flush(void);
extern void kvmalloc(void);

PRIVATE void print_timer_ticks(void);


// Boot page table used in kernel/multiboot_entry_point.S.
// Page directories (and page tables), must start on a page boundary,
// hence the "__aligned__" attribute.
// Use PTE_PS in page directory entry to enable 4Mbyte pages.
__attribute__((__aligned__(PAGE_SIZE)))
//__attribute__((section(".data")))
struct page_dir_struct entrypgdir[NPDENTRIES] = {
        // Map VA's [0, 4MB) to PA's [0, 4MB)
        [0] = {.present_flag=true, .read_write_flag = 1, .user_supervisor_flag = 0, .zero_flag = 0, .page_size_flag = 1, .page_table_base_address = 0},
        // Map VA's [KERNBASE, KERNBASE+4MB) to PA's [0, 4MB)
        [KERN_BASE>>PDXSHIFT] = {.present_flag=true, .read_write_flag = 1, .user_supervisor_flag = 0, .zero_flag = 0, .page_size_flag = 1, .page_table_base_address = 0},
};


struct multiboot_info mboot_info __attribute__ ((section (".data")));

IMPORT void console_init(void);          // from kernel/console.c


INIT_FUNC void gdt_init(void);
INIT_FUNC int start_kernel(struct std_multiboot_info *std_mboot_info, uint32_t magic, uint32_t stack_top);
PRIVATE void print_kernel_context_info(uint32_t total_memory_kb, uint32_t stack_top);

PRIVATE void process_boot_args(struct multiboot_info mb_info);

//PRIVATE void exercise_libkern(void);



//void handle_command(char *cmd) {
////    printk("Received command: %s\n", cmd);
//    if (!strcmp("selectors", cmd)) {
////        print_segment_selectors();
//    } else if (!strcmp("info", cmd)) {
//        print_kernel_context_info(mboot_info.mem_upper, 5);//FIXME 5
//    } else if (!strcmp("ticks", cmd)) {
////        printk("%d\n", get_clock_ticks());
//    } else if (!strcmp("bootargs", cmd)) {
//        printk("%s\n", mboot_info.cmdline);
//    } else if (!strcmp("dani...", cmd)) {
//        printk("%s\n", "te amo!! :)");
//    }
//}



/**
 * This is the main kernel function.
 *
 * @param std_mboot_info Contextual information given by the bootloader.
 * @param magic Number representing the Multiboot bootloader magic number.
 * @see multiboot_entry_point.S file
 */
int start_kernel(struct std_multiboot_info *std_mboot_info, uint32_t magic, uint32_t stack_top) {
    /*
     * Interrupts are still disabled. Do necessary setups, then
     * enable them
     */

    console_init();

    printk("Starting Brunix...\n\n");

//    exercise_libkern();

    verify_loader(magic);

    save_multiboot_info(std_mboot_info, &mboot_info);

    process_boot_args(mboot_info);

    printk("Initializing physical page allocator...\n");
    kinit1(kernel_end, PHYS_TO_VIRT(MB_TO_BYTES(4))); // phys page allocator


//     test physical memory allocator
    char *mem = kalloc();
    if(mem == 0) {
        panic("Out of memory. This should not happen...\n");
    }

//    printk("IRQs...");
//    irq_init();
//    printk("Enabling interrupts...");
//    asm volatile("sti");

//    timer_init(100); // Initialise timer to 100Hz
    print_timer_ticks();

//    printk("Keyboard...");
//    kbd_init();

//    idt_flush();
//
//    printk("Enabling interrupts...");
//    asm volatile("sti");

    print_kernel_context_info(mboot_info.mem_upper, stack_top);


    // ---
//    printk("kalloc() returned page address: %p (virt). Zeroing page...\n", mem);
//    if (VIRT_TO_PHYS(mem) != MB_TO_BYTES(4) - PAGE_SIZE) {
//        panic(">> Unexpected returned address by kalloc(): %p", mem);
//    } temporal comment
//    for (int i = 0; i < 254; i++) {     // requesting PAGE_SIZE * 256 = 1 MBy of memory
//        mem = kalloc();
//        if(mem == 0) {
//            panic("Out of memory. This should not happen...\n");
//        }
//
//        memset(mem, 0, PAGE_SIZE);
//    }
//
//    mem = kalloc();
//    if(mem == 0) {
//        panic("Out of memory. This should not happen...\n");
//    }
//    printk("kalloc() returned page address: %p (virt). Zeroing page...\n", mem);
//    if (VIRT_TO_PHYS(mem) != MB_TO_BYTES(4) - MB_TO_BYTES(1)) {
//        panic("Unexpected returned address by kalloc(): %p", mem);
//    }temporal comment TODO


    kvmalloc();      // kernel page table
    // ---


    //	printk("\nSimulating CPU's exception number 0...\n");
//	asm volatile ("int $0x0");

//	printk("\nSimulating a syscall...\n");
//	asm volatile ("int $0x80");

//    printk("\nGenerating a page fault...");
//    uint32_t *ptr = (uint32_t *)0x400000;
//    uint32_t do_page_fault = *ptr;


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

    printk("Kernel bootstrap stack: %p; start_kernel(): %p\n", stack_top, start_kernel);

    printk("\nSome special kernel symbols:\n");
    printk("  text    %08x (virt)  %08x (phys)\n", kernel_start, VIRT_TO_PHYS_WO(kernel_start));
    printk("  end     %08x (virt)  %08x (phys)\n\n", kernel_end, VIRT_TO_PHYS_WO(kernel_end));
}

//PRIVATE void exercise_libkern(void) {
//    printk("6828 decimal is %o octal!\n", 6828);
//    printk("131 decimal is %b binary!\n", 131);
//    printk("132 decimal is %d signed integer!\n", 132);
//    printk("-133 decimal is %d signed integer!\n", -133);
//    printk("134 decimal is %u unsigned integer!\n", 134);
//    printk("Null string is %s\n", NULL);
//    printk("String brunix is %s\n", "brunix");
//}

PRIVATE void print_timer_ticks() {
    for (int i=0;i<10000000;i++);	// delay
//    printk("kmain", "Clock ticks: %u", get_clock_ticks());
}
