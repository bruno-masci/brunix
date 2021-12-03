#ifndef __ARCH_TRAPS_H__
#define __ARCH_TRAPS_H__

#include <stdint.h>


/*!
 * \struct trapframe
 * \brief Structure describing registers used by the ISR handler.
 */
// Layout of the trap frame built on the stack by the
// hardware and by trapasm.S, and passed to trap().
struct trapframe
{
    // registers as pushed by pusha
    uint32_t edi;
    uint32_t esi;
    uint32_t ebp;
    uint32_t oesp;  // useless & ignored
    uint32_t ebx;
    uint32_t edx;
    uint32_t ecx;
    uint32_t eax;

    // rest of trap frame
    uint16_t gs;
    uint16_t padding1;
    uint16_t fs;
    uint16_t padding2;
    uint16_t es;
    uint16_t padding3;
    uint16_t ds;
    uint16_t padding4;
    uint32_t trap_no;

    // below here defined by x86 hardware
    uint32_t err_code;                          /* error code (if applicable) */
    uint32_t eip;
    uint32_t cs;
    uint16_t padding5;
    uint32_t eflags;

    // below here only when crossing rings, such as from user to kernel
    uint32_t useresp;              /*!< Pushed by the processor automatically */
    uint32_t ss;              /*!< Pushed by the processor automatically */
    uint16_t padding6;
};

typedef void (*trap_handler_t)(struct trapframe *);

void register_interrupt_handler(uint8_t n, trap_handler_t handler);

void isr_install(void);

void trap_handler(struct trapframe *tf);


/* Interrupts/Exceptions */
enum {
    X86_TRAP_DE = 0,	/*  0, Divide (by-zero) error */
    X86_TRAP_DB,		/*  1, Debug exceptions */
    X86_TRAP_NMI,		/*  2, Non-maskable interrupt */
    X86_TRAP_BP,		/*  3, Breakpoint */
    X86_TRAP_OF,		/*  4, Overflow */
    X86_TRAP_BR,		/*  5, Bound check */
    X86_TRAP_UD,		/*  6, Invalid opcode */
    X86_TRAP_NM,		/*  7, Coprocessor not available */
    X86_TRAP_DF,		/*  8, Double fault */
    X86_TRAP_OLD_MF,	/*  9, (reserved) */
    X86_TRAP_TS,		/* 10, Invalid TSS */
    X86_TRAP_NP,		/* 11, Segment not present */
    X86_TRAP_SS,		/* 12, Stack exception */
    X86_TRAP_GP,		/* 13, General protection */
    X86_TRAP_PF,		/* 14, Page fault */
    X86_TRAP_SPURIOUS,	/* 15, (reserved) */
    X86_TRAP_MF 		/* 16, Coprocessor error */
		/* 17-31, (reserved) */
		/* 32-255, Available for external interrupts */
};


#endif /* #define __ARCH_TRAPS_H__ */