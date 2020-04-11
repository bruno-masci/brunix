/**
 * @author Bruno Masci
 * @brief Some common kernel definitions
 */

#ifndef __DEFS_H__
#define __DEFS_H__


#define PRIVATE static

/* Extracted from https://graphics.stanford.edu/~seander/bithacks.html#RoundUpPowerOf2 */
static inline long roundup_binary(long n) {
    n--;
    n |= n >> 1;
    n |= n >> 2;
    n |= n >> 4;
    n |= n >> 8;
    n |= n >> 16;
    return ++n;
}

#define MB_TO_BYTES(val) (val * 1024 * 1024)

// FIXME: borrar. solo lo usa vm.c
// number of elements in fixed-size array
#define NELEM(x) (sizeof(x)/sizeof((x)[0]))


#endif /* !__DEFS_H__ */