#include <stdint.h>                 // for uint32_t

#include <arch/x86/processor.h>     // for lgdt()
#include <arch/x86/memlayout.h>     // for VIRT_TO_PHYS, phys_addr_t
#include <arch/x86/segment.h>       // for __KERNEL_CS_SELECTOR, __KERNEL_DS_SELECTOR
#include <arch/x86/mmu.h>
#include <arch/x86/paging.h>

#include <brunix/defs.h>            // for PRIVATE, INIT_FUNC



// Boot page table used in kernel/multiboot_entry_point.S.
// Page directories (and page tables), must start on a page boundary,
// hence the "__aligned__" attribute.
// Use PTE_PS in page directory entry to enable 4Mbyte pages.
__attribute__((__aligned__(PAGE_SIZE)))
struct page_dir_struct entrypgdir[NPDENTRIES];


void load_page_directory(struct page_dir_struct *addr) {
    uint32_t cr4 = read_cr4();
    lcr4(cr4 | MMU_CR4_PSE);

    lcr3((phys_addr_t) addr);
}

void enable_paging() {
//    uint32_t cr4 = read_cr4();
    uint32_t cr0 = read_cr0();

//    printk("CR0 %b  -  CR3: %x\n", (cr0), read_cr3());

//    lcr4(cr4 | MMU_CR4_PSE);
    lcr0(cr0 | MMU_CR0_PG | MMU_CR0_WP);
}

void init_paging() {
    entrypgdir[0].present_flag = 1;
    entrypgdir[0].read_write_flag = 1;
    entrypgdir[0].user_supervisor_flag = 0;
    entrypgdir[0].zero_flag = 0;
    entrypgdir[0].page_size_flag = 1;   // enable 'big' (4 MiB) pages
    entrypgdir[0].page_table_base_address = 0;  // map VA's [0, 4MB) to PA's [0, 4MB)

//    for (int i = 1; i < NPDENTRIES; ++i) {
//        entrypgdir[i].present_flag = 0;
//    }

//    entrypgdir[KERN_BASE>>PDXSHIFT].page_table_base_address = 0;  // map VA's [0, 4MB) to PA's [0, 4MB)

    printk("entrypgdir address: %p...\n", VIRT_TO_PHYS_WO(entrypgdir));
    phys_addr_t addr = (phys_addr_t) VIRT_TO_PHYS_WO(&entrypgdir);  //FIXME con esta anda; con _WO no!!
    printk("addr address: %x...\n", addr);
    load_page_directory((struct page_dir_struct *) &entrypgdir);
    enable_paging();
}
