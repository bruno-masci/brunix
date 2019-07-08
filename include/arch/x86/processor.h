/**
 * @author Bruno Masci
 * @brief CPU-specific functions
 *
 * This file contains structures and functions related to CPU-specific assembler commands.
 */

#ifndef __ARCH_PROCESSOR_H__
#define __ARCH_PROCESSOR_H__


#define cli() asm ("cli")

//TODO explicar el while! NMI?
#define halt()                      \
do {                                \
    asm ("hlt");                    \
} while (1)


#endif /* __ARCH_PROCESSOR_H__ */