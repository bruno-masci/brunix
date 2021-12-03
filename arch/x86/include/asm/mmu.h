// This file contains definitions for the
// x86 memory management unit (MMU).


//#define PAGE_SIZE          4096    // bytes mapped by a page      REDEFINED TODO


#ifndef __ASSEMBLER__
#include <stdint.h>     // for uint32_t
#include <stdbool.h>     // for bool
#endif


#ifdef __ASSEMBLER__

// Application segment type bits
#define STA_X		0x8	    // Executable segment
#define STA_E		0x4	    // Expand down (non-executable segments)
#define STA_C		0x4	    // Conforming code segment (executable only)
#define STA_W		0x2	    // Writeable (non-executable segments)
#define STA_R		0x2	    // Readable (executable segments)
#define STA_A		0x1	    // Accessed

#endif


//#define PAGE_SIZE          4096    // bytes mapped by a page

#define PGROUNDDOWN(a) (((a)) & ~((uint32_t)(PAGE_SIZE-1)))


// TODO temporal: Control Register flags
#define MMU_CR0_PE          0x00000001      // Protection Enable
#define MMU_CR0_WP          0x00010000      // Write Protect
#define MMU_CR0_PG          0x80000000      // Paging

#define MMU_CR4_PSE         0x00000010      // Page size extension


// Page table/directory entry flags.
#define PTE_P           0x001   // Present
#define PTE_W           0x002   // Writeable
#define PTE_U           0x004   // User

#define NPDENTRIES      1024    // # directory entries per page directory
#define PTE_PS          0x080   // Page Size

// A virtual address 'la' has a three-part structure as follows:
//
// +--------10------+-------10-------+---------12----------+
// | Page Directory |   Page Table   | Offset within Page  |
// |      Index     |      Index     |                     |
// +----------------+----------------+---------------------+
//  \--- PDX(va) --/ \--- PTX(va) --/

// page directory index
#define PDX(va)         (((uint32_t)(va) >> PDXSHIFT) & 0x3FF)

// page table index
#define PTX(va)         (((uint32_t)(va) >> PTXSHIFT) & 0x3FF)

#define PTXSHIFT        12      // offset of PTX in a linear address
#define PDXSHIFT        22      // offset of PDX in a linear address


// Page table/directory entry flags.
#define PTE_P           0x001   // Present
#define PTE_W           0x002   // Writeable
#define PTE_U           0x004   // User

// Address in page table or page directory entry
#define PTE_ADDR(pte)   ((uint32_t)((uint32_t)(pte)) & ((uint32_t)~0xFFF))


#define GDT_ENTRIES	6   // = 2 + 1


// various segment selectors.
#define SEG_KCODE 1  // kernel code
#define SEG_KDATA 2  // kernel data+stack
#define SEG_UCODE 3  // user code
#define SEG_UDATA 4  // user data+stack
#define SEG_TSS   5  // this process's task state