/**
 * @author Bruno Masci
 * @brief Inline functions for processor's I/O operations.
 * @see https://wiki.osdev.org/I/O_Ports
 */

#ifndef __ARCH_IO_H__
#define __ARCH_IO_H__


#include <stdint.h>     // for uint8_t, uint16_t


// Writes a byte to a port
static inline void outb(uint16_t port, uint8_t data) {
    asm volatile("out %0,%1" : : "a" (data), "d" (port));
}


#endif /* __ARCH_IO_H__ */