/**
 * @author Bruno Masci
 * @brief Some common kernel definitions
 */

#ifndef __DEFS_H__
#define __DEFS_H__


#include <stdint.h>         // for uint32_t

#define EXPORT              // Indicates a symbol is exported to other source files
#define IMPORT extern       // Indicates a symbol es imported from other source files
#define PRIVATE static      // Indicates a symbol is private to the containing source file


/*
 * Extracted from https://graphics.stanford.edu/~seander/bithacks.html#RoundUpPowerOf2
 * NOTE: It only works for 32-bits numbers!
 * CORREGIR Si no es exaxtamente multiplo de 2
 */
static inline uint32_t roundup_binary(uint32_t n) {
    n--;
    n |= n >> 1;
    n |= n >> 2;
    n |= n >> 4;
    n |= n >> 8;
    n |= n >> 16;
    return ++n;
}

/* Check if the bit BIT in FLAGS is set. */
#define CHECK_FLAG(flags,bit)   ((flags) & (1 << (bit)))

#define MB_TO_BYTES(val) (val * 1024 * 1024)

// number of elements in fixed-size array
#define NELEM(x) (sizeof(x)/sizeof((x)[0]))


#endif /* !__DEFS_H__ */
