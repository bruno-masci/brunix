/**
 * @author Bruno Masci
 * @brief Inline functions for processor's IO operations.
 */

#ifndef __ARCH_IO_H__
#define __ARCH_IO_H__


#include <stdint.h>

inline static uint8_t inb(uint16_t port) {
    uint8_t data;

    asm volatile("in %1,%0" : "=a" (data) : "d" (port));
    return data;
}

inline static void outb(uint16_t port, uint8_t data) {
    asm volatile("out %0,%1" : : "a" (data), "d" (port));
}


#endif /* __ARCH_IO_H__ */