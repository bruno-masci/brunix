/**
 * @author Bruno Masci
 * @brief CPU-specific functions
 *
 * This file contains structures and functions related to CPU-specific assembler commands.
 */

#ifndef __ARCH_PROCESSOR_H__
#define __ARCH_PROCESSOR_H__


// Clears (maskable) interrupts
static inline void cli(void) {
    asm ("cli");
}

// Halts the CPU
static inline void halt(void) {
    asm ("hlt");
}


#endif /* __ARCH_PROCESSOR_H__ */
