/**
 * @author Bruno Masci
 * @brief x86 memory layout
 */

#ifndef __ARCH_MEMLAYOUT_H__
#define __ARCH_MEMLAYOUT_H__

#ifndef __ASSEMBLER__
#include <stdint.h>     // for uint32_t
typedef uint32_t phys_addr_t;
#endif

#define EXT_MEM_BASE        0x00100000      // Start of "extended" memory (1 MiB)
//#define EXT_MEM_BASE        0x00100000      // Start of "extended" memory (1 MiB)
#define PHYSTOP             0xE000000       // Top physical memory
#define DEVSPACE            0xFE000000      // Other devices are at high addresses

#if PHYSTOP % 4096
#error PHYSTOP must be page aligned!
#endif

// Key addresses for address space layout (see kmap in vm.c for layout)
#define KERN_BASE 0x00000000         //0x80000000         // First kernel virtual address
#define KERN_LINK (KERN_BASE + EXT_MEM_BASE)  // Address where kernel is linked at

#define VIRT_TO_PHYS(a) ((phys_addr_t) (((uint32_t) (a)) - KERN_BASE))
#define PHYS_TO_VIRT(a) (((void *) (a)) + KERN_BASE)
#define P2V(a) ((void *)(((char *) (a)) + KERN_BASE))
//
#define VIRT_TO_PHYS_WO(x) ((x) - KERN_BASE)    // same as VIRT_TO_PHYS, but without casts
#define PHYS_TO_VIRT_WO(x) ((x) + KERN_BASE)    // same as P2V, but without casts


#endif /* __ARCH_MEMLAYOUT_H__ */
