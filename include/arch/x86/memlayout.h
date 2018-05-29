/**
 * @author Bruno Masci
 * @brief x86 memory layout
 */

#ifndef __ARCH_MEMLAYOUT_H__
#define __ARCH_MEMLAYOUT_H__


#define EXT_MEM_BASE       0x00100000            // Start of extended memory (1 MiB)
//#define PHYSTOP 0xE000000           // Top physical memory
//#define DEVSPACE 0xFE000000         // Other devices are at high addresses

// Key addresses for address space layout (see kmap in vm.c for layout)
#define KERN_BASE 0x80000000         // First kernel virtual address
#define KERN_LINK (KERNBASE+EXTMEM)  // Address where kernel is linked

//#define V2P(a) (((uint) (a)) - KERNBASE)
//#define P2V(a) (((void *) (a)) + KERNBASE)
//
//#define V2P_WO(x) ((x) - KERNBASE)    // same as V2P, but without casts
//#define P2V_WO(x) ((x) + KERNBASE)    // same as P2V, but without casts


#endif /* __ARCH_MEMLAYOUT_H__ */
