/**
 * @author Bruno Masci
 * @brief Kernel's entry page directory
 *
 * This file contains the entry (only for bootstrapping) page directory.
 */

#include <asm/memlayout.h>     // for VIRT_TO_PHYS_WO
#include <asm/mmu.h>
#include <asm/page.h>
#include <asm/paging.h>



// Boot page table used in kernel/multiboot_entry_point.S.
// Page directories (and page tables), must start on a page boundary,
// hence the "__aligned__" attribute.
// Use PTE_PS in page directory entry to enable 4Mbyte pages.
__attribute__((__aligned__(PAGE_SIZE)))
//__attribute__((section(".data")))
struct page_dir_struct entrypgdir[NPDENTRIES] = {
        // Map VA's [0, 4MB) to PA's [0, 4MB)
        [0] = {.present_flag=true, .read_write_flag = 1, .user_supervisor_flag = 1, .zero_flag = 0, .page_size_flag = 1, .page_table_base_address = 0},
        // Map VA's [KERNBASE, KERNBASE+4MB) to PA's [0, 4MB)
        [KERN_BASE>>PDXSHIFT] = {.present_flag=true, .read_write_flag = 1, .user_supervisor_flag = 1, .zero_flag = 0, .page_size_flag = 1, .page_table_base_address = 0},
};
