/*
 * Copyright (c) 2010, Stefan Lankes, RWTH Aachen University
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *    * Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 *    * Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in the
 *      documentation and/or other materials provided with the distribution.
 *    * Neither the name of the University nor the names of its contributors
 *      may be used to endorse or promote products derived from this
 *      software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/** 
 * @author Stefan Lankes
 * @author Bruno Masci
 * @brief CPU-specific functions
 *
 * This file contains structures and functions related to CPU-specific assembler commands.
 */

#ifndef __ARCH_PROCESSOR_H__
#define __ARCH_PROCESSOR_H__

#include <brunix/stddef.h>


/** @brief Read out time stamp counter
 *
 * The rdtsc asm command puts a 64 bit time stamp value
 * into EDX:EAX.
 *
 * @return The 64 bit time stamp value
 */
inline static uint64_t rdtsc(void)
{
	uint64_t x;
	asm volatile ("rdtsc" : "=A" (x));
	return x;
}

/** @brief Flush cache
 *
 * The wbinvd asm instruction which stands for "Write back and invalidate"
 * is used here
 */
inline static void flush_cache(void) {
	asm volatile ("wbinvd" : : : "memory");
}

/** @brief Invalidate cache
 *
 * The invd asm instruction which invalidates cache without writing back
 * is used here
 */
inline static void invalid_cache(void) {
	asm volatile ("invd");
}

/// Force strict CPU ordering, serializes load and store operations.
inline static void mb(void) { asm volatile("mfence" ::: "memory"); }
/// Force strict CPU ordering, serializes load operations.
inline static void rmb(void) { asm volatile("lfence" ::: "memory"); }
/// Force strict CPU ordering, serializes store operations.
inline static void wmb(void) { asm volatile("sfence" ::: "memory"); }

/** @brief search the first most significant bit
 *
 * @param i source operand
 * @return
 * - first bit, which is set in the source operand
 * - invalid value, if not bit ist set
 */
static inline size_t msb(size_t i)
{
	size_t ret;

	if (!i)
		return (sizeof(size_t)*8);
	asm volatile ("bsr %1, %0" : "=r"(ret) : "r"(i) : "cc");

	return ret;
}

/** @brief search the least significant bit
 *
 * @param i source operand
 * @return
 * - first bit, which is set in the source operand
 * - invalid value, if not bit ist set
 */
static inline size_t lsb(size_t i)
{
	size_t ret;

	if (!i)
		return (sizeof(size_t)*8);
	asm volatile ("bsf %1, %0" : "=r"(ret) : "r"(i) : "cc");

	return ret;
}

/// A one-instruction-do-nothing
#define NOP1	asm  volatile ("nop")
/// A two-instruction-do-nothing
#define NOP2	asm  volatile ("nop;nop")
/// A four-instruction-do-nothing
#define NOP4	asm  volatile ("nop;nop;nop;nop")
/// A eight-instruction-do-nothing
#define NOP8	asm  volatile ("nop;nop;nop;nop;nop;nop;nop;nop")

#define HALT	asm  volatile ("hlt")


#endif /* __ARCH_PROCESSOR_H__ */
