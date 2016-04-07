#ifndef __ARCH_IO_H__
#define __ARCH_IO_H__

#include <brunix/stddef.h>

void outb(uint16_t port, uint8_t value);

uint8_t inb(uint16_t port);

uint16_t inw(uint16_t port);

#endif /* #define __ARCH_IO_H__ */
