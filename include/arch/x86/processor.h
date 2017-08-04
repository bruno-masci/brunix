/**
 * @author Bruno Masci
 * @brief CPU-specific functions
 *
 * This file contains structures and functions related to CPU-specific assembler commands.
 */

#ifndef __ARCH_PROCESSOR_H__
#define __ARCH_PROCESSOR_H__

#include "vga.h"

#define cli()    asm ("cli")
#define sti()    asm ("sti")

#define halt()                      \
do {                                \
    vga_puts("System halted.");     \
    asm ("hlt");                    \
} while (1)

#endif /* __ARCH_PROCESSOR_H__ */