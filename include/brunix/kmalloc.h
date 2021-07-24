#ifndef __BRUNIX_KMALLOC_H__
#define __BRUNIX_KMALLOC_H__


#ifdef __KERNEL__


// Free the page of physical memory pointed at by v,
// which normally should have been returned by a
// call to kmalloc().  (The exception is when
// initializing the allocator; see kinit above.)
void kfree(const void *vaddr);

// Allocate one 4096-byte page of physical memory.
// Returns a pointer that the kernel can use.
// Returns 0 if the memory cannot be allocated.
void * kmalloc(void);


#endif  /* __KERNEL__ */


#endif  /* __BRUNIX_KMALLOC_H__ */
