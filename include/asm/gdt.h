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
 * @file arch/x86/include/asm/gdt.h
 * @brief Definitions and functions related to segmentation
 * @author Stefan Lankes
 *
 * This file defines the interface for segmentation as like structures to describe segments.\n
 * There are also some other gdt-private functions in the gdt.c file defined.
 */

#ifndef __ARCH_GDT_H__
#define __ARCH_GDT_H__

#include <stdint.h>     // for uint8_t, uint16_t, uint32_t

#define GDT_ENTRIES	3   // = 2 + 1

#define GDT_SEGMENT_LIMIT	0xFFFFFFFF


/// This segment is a data segment
#define GDT_FLAG_DATASEG	0x02
/// This segment is a code segment
#define GDT_FLAG_CODESEG	0x0a
#define GDT_FLAG_TSS		0x09
#define GDT_FLAG_TSS_BUSY	0x02

#define GDT_FLAG_SEGMENT	0x10
/// Privilege level: Ring 0
#define GDT_FLAG_RING0		0x00
/// Privilege level: Ring 3
#define GDT_FLAG_RING3		0x60
/// Segment is present
#define GDT_FLAG_PRESENT    0x80
/// Segment was accessed
#define GDT_FLAG_ACCESSED   0x01
/**
 * @brief Granularity of segment limit
 * - set: segment limit unit is 4 KIB (page size)
 * - not set: unit is bytes
 */
#define GDT_FLAG_4K_GRAN	0x80
/**
 * @brief Default operand size
 * - set: 32 bit
 * - not set: 16 bit
 */
#define GDT_FLAG_16_BIT		0x00
#define GDT_FLAG_32_BIT		0x40
#define GDT_FLAG_64_BIT		0x20

/** @brief Defines a GDT entry
 *
 * A global descriptor table entry consists of:
 * - 32 bit base address (chunkwise embedded into this structure)
 * - 20 bit limit
 */

struct gdt_desc_access_struct {
    // type section
    uint8_t  accessed_flag : 1;
    uint8_t  readable_flag : 1;
    uint8_t  direction_flag : 1;
    uint8_t  executable_flag : 1;

    uint8_t  non_system_segment_flag : 1;
    uint8_t  dpl : 2;   // descriptor privilege level (rings)
    uint8_t  present_flag : 1;
};// __attribute__((packed));

struct gdt_desc_flags_struct {
    uint8_t  reserved_unused : 2;
    uint8_t  size_flag : 1;
    uint8_t  granularity_flag : 1;
};// __attribute__((packed));

struct gdt_desc_struct {
    uint16_t limit_low_16;
    uint16_t base_low_16;
    uint8_t  base_middle_8;
    uint8_t access_8;
    uint16_t limit_high_4 : 4;
    uint8_t flags_4 : 4;
    uint8_t  base_high_8;
} __attribute__((packed));



/** @brief defines the GDT pointer structure
 *
 * This structure tells the address and size of the table.
 */
struct gdt_ptr_struct {
    /// Table limit: Size of the table in bytes (not the number of entries!)
    uint16_t limit;
    /// Linear base address: Address of the table (the address of the first gdt_entry_t struct)
    uint32_t base;
} __attribute__ ((packed));


struct tss {
    uint16_t previous_task, __previous_task_unused;
    uint32_t esp0;
    uint16_t ss0, __ss0_unused;
    uint32_t esp1;
    uint16_t ss1, __ss1_unused;
    uint32_t esp2;
    uint16_t ss2, __ss2_unused;
    uint32_t cr3;
    uint32_t eip, eflags, eax, ecx, edx, ebx, esp, ebp, esi, edi;
    uint16_t es, __es_unused;
    uint16_t cs, __cs_unused;
    uint16_t ss, __ss_unused;
    uint16_t ds, __ds_unused;
    uint16_t fs, __fs_unused;
    uint16_t gs, __gs_unused;
    uint16_t ldt_selector, __ldt_sel_unused;
    uint16_t debug_flag, io_map;
} __attribute__ ((packed));


#endif /* __ARCH_GDT_H__ */
