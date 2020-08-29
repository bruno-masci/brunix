/**
 * @author Bruno Masci
 * @brief CPU-specific functions
 *
 * This file contains structures and functions related to CPU-specific assembler commands.
 */

#ifndef __ARCH_PROCESSOR_H__
#define __ARCH_PROCESSOR_H__


#include <stdbool.h>    // for true


// Clears (maskable) interrupts
static inline void cli(void) {
    asm ("cli");
}

// Halts the CPU, and keeps doing it in case some NMI event is generated
static inline void __attribute__ ((noreturn)) halt(void) {
    do {
        asm ("hlt");
    } while (true);
}


#endif /* __ARCH_PROCESSOR_H__ */