/**
 * @author Bruno Masci
 * @brief CPU-specific functions
 *
 * This file contains structures and functions related to CPU-specific assembler commands.
 */

#ifndef __ARCH_PROCESSOR_H__
#define __ARCH_PROCESSOR_H__

#include <stdint.h>     // for uint32_t


#define cli() asm ("cli")

//TODO explicar el while! NMI?
#define halt()                      \
do {                                \
    asm ("hlt");                    \
} while (1)


static inline void
lcr3(uint32_t val)
{
    asm volatile("movl %0,%%cr3" : : "r" (val));
}

#endif /* __ARCH_PROCESSOR_H__ */