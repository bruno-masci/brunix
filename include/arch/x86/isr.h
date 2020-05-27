#ifndef __ARCH_ISR_H__
#define __ARCH_ISR_H__

#include <stdint.h>
//#include <stddef.h>


// These extern directives let us access the addresses of our ASM ISR handlers.
extern void isr0();
extern void isr1();
extern void isr2();
extern void isr3();
extern void isr4();
extern void isr5();
extern void isr6();
extern void isr7();
extern void isr8();
extern void isr9();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();
/*
extern void isr0x80();
*/


/*!
 * \struct registers_t
 * \brief Structure describing registers used by the ISR handler.
 */
struct registers_t
{
    /*!< Pushed by pushal */
    uint32_t edi, esi, ebp,
    oesp,    // useless & ignored
    ebx, edx, ecx, eax;

    // rest of trap frame
    uint16_t gs;
    uint16_t padding1;
    uint16_t fs;
    uint16_t padding2;
    uint16_t es;
    uint16_t padding3;
    uint16_t ds;
    uint16_t padding4;
    uint32_t int_no;                          /*!< Interrupt number */

    // below here defined by x86 hardware
    /*!< Pushed by the processor automatically */
    uint32_t err_code;   // (if applicable)
    uint32_t eip;
    uint16_t cs;
    uint16_t padding5;
    uint32_t eflags;

    // extra pushed by the processor only when crossing rings, such as from user to kernel
    uint32_t esp;
    uint16_t ss;
    uint16_t padding6;
};

typedef void (*isr_t)(struct registers_t *);

void register_interrupt_handler(uint8_t n, isr_t handler);

void isr_install(void);

void isr_handler(struct registers_t *regs);


#endif /* #define __ARCH_ISR_H__ */
