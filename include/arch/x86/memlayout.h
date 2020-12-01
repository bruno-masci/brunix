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
typedef uintptr_t phys_addr_t;

#define VIRT_TO_PHYS(a) ((phys_addr_t) (((uintptr_t) (a)) - KERN_BASE))

#endif  /* ! __ASSEMBLER__ */


#define EXT_MEM_BASE    0x00100000          // Start of physical "extended" memory (1 MiB)

#define KERN_BASE 0x00000000         //0x80000000         // First kernel virtual address

#define KERN_LINK (KERN_BASE + EXT_MEM_BASE)  // Address where kernel is linked at

#define VIRT_TO_PHYS_WO(x) ((x) - KERN_BASE)    // same as VIRT_TO_PHYS, but without casts

#endif /* __ARCH_MEMLAYOUT_H__ */



//#define PHYS_TO_VIRT(a) (((void *) (a)) + KERN_BASE)
//#define P2V(a) ((void *)(((char *) (a)) + KERN_BASE))

//#define VIRT_TO_PHYS_WO(x) ((x) - KERN_BASE)    // same as VIRT_TO_PHYS, but without casts
//#define PHYS_TO_VIRT_WO(x) ((x) + KERN_BASE)    // same as P2V, but without casts