/**
 * @author Bruno Masci
 * @brief Inline functions for processor IO operations.
 */

#ifndef __ARCH_IO_H__
#define __ARCH_IO_H__

#include <brunix/stddef.h>


// Write a byte out to the specified port.
inline static void outb(uint16_t port, uint8_t value) {
    asm volatile("outb %1, %0"::"dN"(port), "a"(value));
}

// Read a byte in from the specified port.
inline static uint8_t inb(uint16_t port) {
    uint8_t rv;
    asm volatile("inb %1, %0":"=a"(rv):"dN"(port));
    return rv;
}

// Read a word in from the specified port.
inline static uint16_t inw(uint16_t port) {
    uint16_t rv;
    asm volatile("inw %1, %0":"=a"(rv):"dN"(port));
    return rv;
}

// TODO Check whether outl and inl instructions (double words) are required


#endif /* __ARCH_IO_H__ */



