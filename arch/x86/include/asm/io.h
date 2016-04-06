#ifndef INCLUDE_IO_H
#define INCLUDE_IO_H

#include <brunix/stddef.h>

void outb(uint16_t port, uint8_t value);

uint8_t inb(uint16_t port);

uint16_t inw(uint16_t port);

#endif /* INCLUDE_IO_H */
