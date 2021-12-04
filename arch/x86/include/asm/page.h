/**
 * @brief Definitions and functions related to x86 paging's pages
 * @author Bruno Masci
 *
 * This file defines page related stuff.
 */

#ifndef __ARCH_PAGE_H__
#define __ARCH_PAGE_H__

#include <stdint.h>     // for uint32_t
#include <stdbool.h>

#include "traps.h"    // for struct trapframe



#define PAGE_SIZE					0x1000 // 4 KB
#define	RAM_MAXPAGE					0x100000
#define PAGE(addr)					(uint32_t)(((uint32_t)addr) >> 12)

/* PAGE_SHIFT determines the page size */
#define PAGE_SHIFT	12
//#define PAGE_SIZE	(1UL << PAGE_SHIFT)
#define PAGE_MASK	(~(PAGE_SIZE-1))


/* to align the pointer to the (next) page boundary */
#define PAGE_ROUND_DOWN(addr)	(((uint32_t)(addr)) & -PAGE_SIZE)
#define PAGE_ROUND_UP(addr)	((((uint32_t)(addr))+PAGE_SIZE-1) & ((uint32_t)PAGE_MASK))


//enum PageSize {NORMAL, HUGE};



#ifndef __ASSEMBLER__
static inline bool is_page_aligned(const char *v) {
    return ((uint32_t) v % PAGE_SIZE) == 0;
}
#endif


//#ifdef __KERNEL__

/*
 * These are used to make use of C type-checking..
 */
typedef struct { unsigned long _pte; } pte_t;
typedef struct { unsigned long _pde; } pde_t;
typedef struct { unsigned long _pde; } pgprot_t;

#define pte_val(pte) ((pte)->_pte)
#define pde_val(pde) ((pde)->_pde)

#define pte_set(pte, val) ((pte)->_pte = val);
#define pde_set(pde, val) ((pde)->_pde = val);

#define __pgprot(x)	((pgprot_t) { (x) } )


#define invalidate() \
__asm__ __volatile__("movl %%cr3,%%eax\n\tmovl %%eax,%%cr3": : :"ax")


//#endif




#endif /* __ARCH_PAGE_H__ */
