#include "kalloc.h"

#include <stdint.h>
#include <stdbool.h>                // for bool

#include <arch/x86/mmu.h>
#include <arch/x86/memlayout.h>

#include <brunix/defs.h>            // for PRIVATE
#include <brunix/kernel.h>
#include <brunix/string.h>
#include <brunix/console.h>         // for cprintf()


extern const char kernel_end[]; // first address after kernel loaded from ELF file
// defined by the kernel linker script in kernel.ld


PRIVATE void free_range(const void *vstart, const void *vend);


struct page_node {
    struct page_node *next;
};

PRIVATE struct page_node *freelist = NULL;
PRIVATE long free_pages_count = 0;


// Initialization happens in two phases.
// 1. main() calls kinit1() while still using entrypgdir to place just
// the pages mapped by entrypgdir on free list.
// 2. main() calls kinit2() with the rest of the physical pages
// after installing a full page table that maps them on all cores.
void
kinit1(const void *vstart, const void *vend)
{
    free_range(vstart, vend);
    cprintf("total free pages: %d! bytes: %d\n", free_pages_count, free_pages_count * PAGE_SIZE);
}

/** Free the page of physical memory pointed at by v, which normally should
 * have been returned by a call to kalloc(). (The exception is when
 * initializing the allocator.
 * @see kinit1()
 */
void kfree(const char *vaddr) {
    if(!is_page_aligned(vaddr) || vaddr < kernel_end || VIRT_TO_PHYS(vaddr) >= PHYSTOP)
        panic("kfree");

    // Fill with junk to catch dangling refs.
    memset(vaddr, 1, PAGE_SIZE);

    struct page_node *node = (struct page_node *) vaddr;
    node->next = freelist;
    freelist = node;

    free_pages_count++;
}

// Allocate one 4096-byte page of physical memory.
// Returns a pointer that the kernel can use.
// Returns 0 if the memory cannot be allocated.
//TODO por que devuelve char* ??
char * kalloc(void) {
    struct page_node *node = freelist;
    if (node)
        freelist = node->next;

    return ((char *) node);
}

// start at kernel end address
PRIVATE void free_range(const void *vstart, const void *vend) {
    char *p = (char *) PAGE_ROUND_UP((uint32_t) vstart);
    for(; p + PAGE_SIZE <= (char *) vend; p += PAGE_SIZE)
        kfree(p);
}