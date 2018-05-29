/**
 * @author Bruno Masci
 * @brief Inline functions for processor's IO operations.
 */

#ifndef __ARCH_IO_H__
#define __ARCH_IO_H__


#include <stdint.h>

// Write a byte out to the specified port.
inline static void outb(uint16_t port, uint8_t value) {
    asm volatile("outb %1, %0" : : "dN"(port), "a"(value));
}


#endif /* __ARCH_IO_H__ */