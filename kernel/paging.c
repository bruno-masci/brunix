#include <stdint.h>                 // for uint32_t

#include <arch/x86/processor.h>     // for lgdt()
#include <arch/x86/memlayout.h>     // for VIRT_TO_PHYS, phys_addr_t
#include <arch/x86/segment.h>       // for __KERNEL_CS_SELECTOR, __KERNEL_DS_SELECTOR
#include <arch/x86/mmu.h>
#include <arch/x86/paging.h>

#include <brunix/defs.h>            // for PRIVATE, INIT_FUNC




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
