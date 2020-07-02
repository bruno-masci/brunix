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


static inline uint32_t
read_ebp(void)
{
    uint32_t ebp;
    asm volatile("movl %%ebp,%0" : "=r" (ebp));
    return ebp;
}

static inline void
lcr3(uint32_t val)
{
    asm volatile("movl %0,%%cr3" : : "r" (val));
}

static inline void
lgdt(void *p)
{
    asm volatile("lgdt (%0)" : : "r" (p));
}

static inline void
lidt(void *p)
{
    asm volatile("lidt (%0)" : : "r" (p));
}

static inline void
ltr(uint16_t sel)
{
    asm volatile("ltr %0" : : "r" (sel));
}

/** @brief Read out time stamp counter
 *
 * The rdtsc asm command puts a 64 bit time stamp value
 * into EDX:EAX.
 *
 * @return The 64 bit time stamp value
 */
static inline uint64_t rdtsc(void)
{
    uint64_t x;
    asm volatile ("rdtsc" : "=A" (x));
    return x;
}

#endif /* __ARCH_PROCESSOR_H__ */