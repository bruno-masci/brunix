/* From linux source tree */
#ifndef _ASM_X86_SEGMENT_H
#define _ASM_X86_SEGMENT_H


/*
 * Constructor for a conventional segment GDT (or LDT) entry.
 * This is a macro so it can be used in initializers.
 */
#define GDT_ENTRY(flags, base, limit)			\
	((((base)  & _AC(0xff000000,ULL)) << (56-24)) |	\
	 (((flags) & _AC(0x0000f0ff,ULL)) << 40) |	\
	 (((limit) & _AC(0x000f0000,ULL)) << (48-16)) |	\
	 (((base)  & _AC(0x00ffffff,ULL)) << 16) |	\
	 (((limit) & _AC(0x0000ffff,ULL))))



/*
 * Bottom two bits of selector give the ring
 * privilege level
 */
#define SEGMENT_RPL_MASK	0x3

/* User mode is privilege level 3: */
#define USER_RPL		0x3

/* Bit 2 is Table Indicator (TI): selects between LDT or GDT */
#define SEGMENT_TI_MASK		0x4
/* LDT segment has TI set ... */
#define SEGMENT_LDT		0x4
/* ... GDT has it cleared */
#define SEGMENT_GDT		0x0

#define GDT_ENTRY_INVALID_SEG	0


#define GDT_ENTRY_TLS_MIN		6
#define GDT_ENTRY_TLS_MAX 		(GDT_ENTRY_TLS_MIN + GDT_ENTRY_TLS_ENTRIES - 1)

#define GDT_ENTRY_KERNEL_CS		12
#define GDT_ENTRY_KERNEL_DS		13
#define GDT_ENTRY_DEFAULT_USER_CS	14
#define GDT_ENTRY_DEFAULT_USER_DS	15
#define GDT_ENTRY_TSS			16
#define GDT_ENTRY_LDT			17

#define GDT_ENTRY_ESPFIX_SS		26
#define GDT_ENTRY_PERCPU		27
#define GDT_ENTRY_STACK_CANARY		28

#define GDT_ENTRY_DOUBLEFAULT_TSS	31

/*
 * Number of entries in the GDT table:
 */
#define GDT_ENTRIES			32

/*
 * Segment selector values corresponding to the above entries:
 */

#define __KERNEL_CS			(GDT_ENTRY_KERNEL_CS*8)
#define __KERNEL_DS			(GDT_ENTRY_KERNEL_DS*8)
#define __USER_DS			(GDT_ENTRY_DEFAULT_USER_DS*8 + 3)
#define __USER_CS			(GDT_ENTRY_DEFAULT_USER_CS*8 + 3)
#define __ESPFIX_SS			(GDT_ENTRY_ESPFIX_SS*8)





#define IDT_ENTRIES			256
#define NUM_EXCEPTION_VECTORS		32

/* Bitmask of exception vectors which push an error code on the stack: */
#define EXCEPTION_ERRCODE_MASK		0x00027d00

#define GDT_SIZE			(GDT_ENTRIES*8)
#define GDT_ENTRY_TLS_ENTRIES		3
#define TLS_SIZE			(GDT_ENTRY_TLS_ENTRIES* 8)




/*
 * Load a segment. Fall back on loading the zero
 * segment if something goes wrong..
 */
#define loadsegment(seg, value)						\
do {									\
	unsigned short __val = (value);					\
									\
	asm volatile("						\n"	\
		     "1:	movl %k0,%%" #seg "		\n"	\
									\
		     ".section .fixup,\"ax\"			\n"	\
		     "2:	xorl %k0,%k0			\n"	\
		     "		jmp 1b				\n"	\
		     ".previous					\n"	\
									\
		     _ASM_EXTABLE(1b, 2b)				\
									\
		     : "+r" (__val) : : "memory");			\
} while (0)

/*
 * Save a segment register away:
 */
#define savesegment(seg, value)				\
	asm("mov %%" #seg ",%0":"=r" (value) : : "memory")

/*
 * x86-32 user GS accessors:
 */
#define get_user_gs(regs)		(u16)({ unsigned long v; savesegment(gs, v); v; })
#define set_user_gs(regs, v)		loadsegment(gs, (unsigned long)(v))
#define task_user_gs(tsk)		((tsk)->thread.gs)
#define lazy_save_gs(v)		savesegment(gs, (v))
#define lazy_load_gs(v)		loadsegment(gs, (v))



#endif /* _ASM_X86_SEGMENT_H */
