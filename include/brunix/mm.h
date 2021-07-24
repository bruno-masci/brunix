#ifndef __BRUNIX_MM_H__
#define __BRUNIX_MM_H__

#include <asm/page.h>

#ifdef __KERNEL__

/* memory.c & swap.c*/

//#define free_page(addr) free_pages((addr),0)
//extern void free_pages(unsigned long addr, unsigned long order);

extern void show_free_areas(void);
extern unsigned long put_dirty_page(struct task_struct * tsk,unsigned long page,
                                    unsigned long address);

//extern void free_page_tables(struct task_struct * tsk);
//extern void clear_page_tables(struct task_struct * tsk);
//extern int copy_page_tables(struct task_struct * to);
//extern int clone_page_tables(struct task_struct * to);
//extern int unmap_page_range(unsigned long from, unsigned long size);
//extern int remap_page_range(unsigned long from, unsigned long to, unsigned long size, pgprot_t prot);
//extern int zeromap_page_range(unsigned long from, unsigned long size, pgprot_t prot);

//extern void handle_mm_fault(struct vm_area_struct *vma, unsigned long address, int write_access);
//extern void do_wp_page(struct vm_area_struct * vma, unsigned long address, int write_access);
//extern void do_no_page(struct vm_area_struct * vma, unsigned long address, int write_access);

//extern unsigned long paging_init(unsigned long start_mem, unsigned long end_mem);
//extern void mem_init(unsigned long start_mem, unsigned long end_mem);
//extern void show_mem(void);
//extern void oom(struct task_struct * tsk);
//extern void si_meminfo(struct sysinfo * val);

/* vmalloc.c */

//extern void * vmalloc(unsigned long size);
//extern void vfree(void * addr);
//extern int vread(char *buf, char *addr, int count);

/* mmap.c */

//...

/* kmalloc.c */


// Initialization happens in two phases.
// 1. main() calls kinit1() while still using entrypgdir to place just
// the pages mapped by entrypgdir on free list.
// 2. main() calls kinit2() with the rest of the physical pages
// after installing a full page table that maps them on all cores.
void kinit1(const void *vstart, const void *vend);

// Free the page of physical memory pointed at by v,
// which normally should have been returned by a
// call to kmalloc().  (The exception is when
// initializing the allocator; see kinit above.)
void kfree(const void *vaddr);

// Allocate one 4096-byte page of physical memory.
// Returns a pointer that the kernel can use.
// Returns 0 if the memory cannot be allocated.
//TODO por que devuelve char* ??
void * kmalloc(void);


#endif /* __KERNEL__ */

#endif
