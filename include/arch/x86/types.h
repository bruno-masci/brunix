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
// and phys_addr_t to represent physical addresses.
//typedef uint32_t physaddr_t;


typedef uint32_t pde_t;

#endif /* !__ARCH_TYPES_H__ */