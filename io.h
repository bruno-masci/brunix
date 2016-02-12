#ifndef INCLUDE_IO_H
#define INCLUDE_IO_H

#include <types.h>

void outb(u16int port, u8int value);

u8int inb(u16int port);

u16int inw(u16int port);

#endif /* INCLUDE_IO_H */