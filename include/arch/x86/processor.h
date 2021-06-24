/**
 * @author Bruno Masci
 * @brief CPU-specific functions
 *
 * This file contains structures and functions related to CPU-specific assembler commands.
 */

#ifndef __ARCH_PROCESSOR_H__
#define __ARCH_PROCESSOR_H__


#include <stdbool.h>    // for true
#include <stdint.h>

#include <arch/x86/memlayout.h>


static inline void lcr3(phys_addr_t val) {
    asm volatile("movl %0,%%cr3" : : "r" (val));
}

static inline uint32_t read_cr4(void) {
    uint32_t val;
    asm volatile("movl %%cr4,%0" : "=r" (val));
    return val;
}


static inline void lcr4(uint32_t val) {
    asm volatile("movl %0,%%cr4" : : "r" (val));
}

static inline void lcr0(uint32_t val) {
    asm volatile("movl %0,%%cr0" : : "r" (val));
}

static inline uint32_t read_cr0(void) {
    uint32_t val;
    asm volatile("movl %%cr0,%0" : "=r" (val));
    return val;
}

static inline void lgdt(phys_addr_t addr) {
    asm volatile("lgdt (%0)" : : "r" (addr));
}

// Clears (maskable) interrupts
static inline void cli(void) {
    asm ("cli");
}

// Halts the CPU
static inline void halt(void) {
    asm ("hlt");
}


#endif /* __ARCH_PROCESSOR_H__ */
