//TODO aun no se si dejar este tipo!
/**
 * @author Bruno Masci
 * @brief Some architecture-dependent type definitions
 */

#ifndef __ARCH_TYPES_H__
#define __ARCH_TYPES_H__


#include <stdint.h> // for uint8_t, uint32_t, uintptr_t, etc.


// Pointers and addresses are 32 bits long.
// We use pointer types to represent virtual addresses,
// uintptr_t to represent the numerical values of virtual addresses,
// and physaddr_t to represent physical addresses.
typedef uint32_t physaddr_t;


#endif /* !__ARCH_TYPES_H__ */



/*
#ifndef JOS_INC_TYPES_H
#define JOS_INC_TYPES_H

#ifndef NULL
#define NULL ((void*) 0)
#endif

#include <stdint.h>

// Pointers and addresses are 32 bits long.
// We use pointer types to represent virtual addresses,
// uintptr_t to represent the numerical values of virtual addresses,
// and physaddr_t to represent physical addresses.
typedef int32_t intptr_t;
typedef uint32_t uintptr_t;
typedef uint32_t physaddr_t;

// size_t is used for memory object sizes.
typedef uint32_t size_t;
// ssize_t is a signed version of ssize_t, used in case there might be an
// error return.
typedef int32_t ssize_t;



#endif
*/