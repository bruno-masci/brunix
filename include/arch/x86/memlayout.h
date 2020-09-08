/**
 * @author Bruno Masci
 * @brief x86 memory layout
 * @see /linker.ld.pp
 */

#ifndef __ARCH_MEMLAYOUT_H__
#define __ARCH_MEMLAYOUT_H__


#define EXT_MEM_BASE 0x00100000                 // Start of physical "extended" memory (1 MiB)

#define KERN_BASE 0x00000000                    // First kernel virtual address
#define KERN_LINK (KERN_BASE + EXT_MEM_BASE)    // Address where kernel is linked at


#endif /* __ARCH_MEMLAYOUT_H__ */
