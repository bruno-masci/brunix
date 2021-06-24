// This file contains definitions for the
// x86 memory management unit (MMU).

#ifndef __ASSEMBLER__
#include <stdint.h>     // for uint32_t
#include <stdbool.h>     // for bool
#endif


// System segment type bits
//#define STS_T32A    0x9     // Available 32-bit TSS
#define STS_IG32    0xE     // 32-bit Interrupt Gate
#define STS_TG32    0xF     // 32-bit Trap Gate


//#define PAGE_SIZE          4096    // bytes mapped by a page

#define PAGE_ROUND_UP(sz)  (((sz)+PAGE_SIZE-1) & ~(PAGE_SIZE-1))
#define PGROUNDDOWN(a) (((a)) & ~(PAGE_SIZE-1))


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
#define PTE_ADDR(pte)   ((uint32_t)(pte) & ~0xFFF)




