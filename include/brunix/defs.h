/**
 * @author Bruno Masci
 * @brief Some common kernel definitions
 */

#ifndef __DEFS_H__
#define __DEFS_H__


#define INIT
#define NORETURN
#define EXPORT
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


#endif /* !__DEFS_H__ */