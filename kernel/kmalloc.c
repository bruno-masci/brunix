#include <brunix/kmalloc.h>

#include <stddef.h>                 // for NULL

#include <asm/page.h>               // for PAGE_SIZE
#include <asm/memlayout.h>          // for PHYSTOP

#include <brunix/defs.h>            // for PRIVATE
#include <brunix/kernel.h>          // for panic()
#include <string.h>          // for memset


extern const void *kernel_end; // first address after kernel loaded from ELF file
// defined by the kernel linker script in kernel.ld

void kmalloc_init(const void *vstart, const void *vend);


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
//-->on linux, called kmalloc_init (returns start addr)
void kmalloc_init(const void *vstart, const void *vend) {
    free_range(vstart, vend);
    printk("vstart=%p vend=%p , total free pages: %d! bytes: %d\n", vstart, vend, free_pages_count, free_pages_count * PAGE_SIZE);
}

/** Free the page of physical memory pointed at by v, which normally should
 * have been returned by a call to kmalloc(). (The exception is when
 * initializing the allocator.
 * @see kmalloc_init()
 */
void kfree(const void * const vaddr) {
    if (!is_page_aligned(vaddr) || vaddr < kernel_end || VIRT_TO_PHYS(vaddr) >= PHYSTOP)
        panic("kfree");

    // Fill with junk to catch dangling refs.k
    memset(vaddr, 1, PAGE_SIZE);

    struct page_node *node = (struct page_node *) vaddr;
    node->next = freelist;
    freelist = node;

    free_pages_count++;
//    printk("FREE PAGES: %d\n", free_pages_count);
}

// Allocate one 4096-byte page of physical memory.
// Returns a pointer that the kernel can use.
// Returns 0 if the memory cannot be allocated.
void * kmalloc(void) {
    struct page_node *node = freelist;
    if (node)
        freelist = node->next;

//    printk("\nDEBUG: kmalloc->%p\n", node);
    return ((char *) node);
}

// start at kernel end address
PRIVATE void free_range(const void *vstart, const void *vend) {
    char *p = (char *) PAGE_ROUND_UP((uint32_t) vstart);
    for(; p + PAGE_SIZE <= (char *) vend; p += PAGE_SIZE) {
//        printk("Freeing p=%p\n", p);
        kfree(p);
    }
}
