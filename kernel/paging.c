#include <stdint.h>                 // for uint32_t

#include <asm/processor.h>     // for lgdt()
#include <asm/memlayout.h>     // for VIRT_TO_PHYS, phys_addr_t
#include <asm/segment.h>       // for __KERNEL_CS_SELECTOR, __KERNEL_DS_SELECTOR
#include <asm/mmu.h>
#include <asm/paging.h>

#include <brunix/defs.h>            // for PRIVATE, INIT_FUNC



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


void load_page_directory(struct page_dir_struct *addr) {
    // Turn on page size extension for 4Mbyte pages
    uint32_t cr4 = read_cr4();
    lcr4(cr4 | MMU_CR4_PSE);

    // Set page directory
    lcr3((phys_addr_t) addr);
}

void enable_paging() {
    uint32_t cr0 = read_cr0();

    lcr0(cr0 | MMU_CR0_PG | MMU_CR0_WP);
}

void init_paging2(void) {
//    entrypgdir[0].present_flag = 1;
//    entrypgdir[0].read_write_flag = 1;
//    entrypgdir[0].user_supervisor_flag = 0;
//    entrypgdir[0].zero_flag = 0;
//    entrypgdir[0].page_size_flag = 1;   // enable 'big' (4 MiB) pages
//    entrypgdir[0].page_table_base_address = 0;  // map VA's [0, 4MB) to PA's [0, 4MB)

//    for (int i = 1; i < NPDENTRIES; ++i) {
//        entrypgdir[i].present_flag = 0;
//    }

//    entrypgdir[KERN_BASE>>PDXSHIFT].present_flag = 1;
//    entrypgdir[KERN_BASE>>PDXSHIFT].read_write_flag = 1;
//    entrypgdir[KERN_BASE>>PDXSHIFT].user_supervisor_flag = 0;
//    entrypgdir[KERN_BASE>>PDXSHIFT].zero_flag = 0;
//    entrypgdir[KERN_BASE>>PDXSHIFT].page_size_flag = 1;   // enable 'big' (4 MiB) pages
//    entrypgdir[KERN_BASE>>PDXSHIFT].page_table_base_address = 0;  // map VA's [0, 4MB) to PA's [0, 4MB)
//    entrypgdir[KERN_BASE>>PDXSHIFT].page_table_base_address = 0;  // map VA's [0, 4MB) to PA's [0, 4MB)



    phys_addr_t addr = VIRT_TO_PHYS(&entrypgdir[0]);
    printk("entrypgdir address: %x...\n", addr);
    load_page_directory((struct page_dir_struct *) (addr));
    enable_paging();
}
