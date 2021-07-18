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

#include <asm/isr.h>	// for registers_t



#define PAGE_SIZE					0x1000 // 4 KB
#define	RAM_MAXPAGE					0x100000
#define PAGE(addr)					(uint32_t)(((uint32_t)addr) >> 12)

/* PAGE_SHIFT determines the page size */
#define PAGE_SHIFT	12
//#define PAGE_SIZE	(1UL << PAGE_SHIFT)
#define PAGE_MASK	(~(PAGE_SIZE-1))


/* to align the pointer to the (next) page boundary */
//#define PAGE_ALIGN(addr)	(((addr)+PAGE_SIZE-1)&PAGE_MASK)
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
typedef struct { unsigned long pte; } pte_t2;
typedef struct { unsigned long pmd; } pmd_t2;
typedef struct { unsigned long pgd; } pgd_t2;
typedef struct { unsigned long pgprot; } pgprot_t2;

#define pte_val(x)	((x).pte)
#define pmd_val(x)	((x).pmd)
#define pgd_val(x)	((x).pgd)
#define pgprot_val(x)	((x).pgprot)

#define __pte(x)	((pte_t) { (x) } )
#define __pmd(x)	((pmd_t) { (x) } )
#define __pgd(x)	((pgd_t) { (x) } )
#define __pgprot(x)	((pgprot_t) { (x) } )

//#endif




#endif /* __ARCH_PAGE_H__ */
