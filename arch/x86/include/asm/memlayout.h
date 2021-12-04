/**
 * @author Bruno Masci
 * @brief x86 memory layout
 * @see /linker.ld.pp
 * @see http://wiki.osdev.org/Memory_Map_(x86)
 */

#ifndef __ARCH_MEMLAYOUT_H__
#define __ARCH_MEMLAYOUT_H__


#ifndef __ASSEMBLER__

#include <stdint.h>     // for uintptr_t
#include "memlayout.h"     // for uintptr_t
typedef uint32_t phys_addr_t;



#endif  /* ! __ASSEMBLER__ */



//#define PHYSTOP     ((phys_addr_t) 0xE000000)       // Top physical memory
#define PHYSTOP     256*1024*1024       // Top physical memory
#define DEVSPACE            0xFE000000      // Other devices are at high addresses

//#if PHYSTOP % 4096
//#error PHYSTOP must be page aligned!
//#endif



#define EXT_MEM_BASE    0x00100000                  // Start of physical "extended" memory (1 MiB)

//0x80000000
#define KERN_BASE       0xc0000000                  // First kernel virtual address

#define KERN_LINK       (KERN_BASE + EXT_MEM_BASE)  // Address where kernel is linked at

#define VIRT_TO_PHYS(a) ((phys_addr_t) (((uint32_t)a) - KERN_BASE))
#define PHYS_TO_VIRT(a) ((void *)(((char *) (a)) + KERN_BASE))

#define VIRT_TO_PHYS_WO(x) ((x) - KERN_BASE)    // same as VIRT_TO_PHYS, but without casts

//orig:
//#define V2P(a) (((uint) (a)) - KERNBASE)
//#define P2V(a) ((void *)(((char *) (a)) + KERNBASE))
//
//#define V2P_WO(x) ((x) - KERNBASE)    // same as V2P, but without casts
//#define P2V_WO(x) ((x) + KERNBASE)    // same as P2V, but without casts


#endif /* __ARCH_MEMLAYOUT_H__ */
