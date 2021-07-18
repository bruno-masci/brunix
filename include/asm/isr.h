#ifndef __ARCH_ISR_H__
#define __ARCH_ISR_H__

#include <stdint.h>
//#include <stddef.h>


// These extern directives let us access the addresses of our ASM ISR handlers.
void isr0(void);
void isr1(void);
void isr2(void);
void isr3(void);
void isr4(void);
void isr5(void);
void isr6(void);
void isr7(void);
void isr8(void);
void isr9(void);
void isr10(void);
void isr11(void);
void isr12(void);
void isr13(void);
void isr14(void);
void isr15(void);
void isr16(void);
void isr17(void);
void isr18(void);
void isr19(void);
void isr20(void);
void isr21(void);
void isr22(void);
void isr23(void);
void isr24(void);
void isr25(void);
void isr26(void);
void isr27(void);
void isr28(void);
void isr29(void);
void isr30(void);
void isr31(void);

void isr0x80(void);



/*!
 * \struct registers_t
 * \brief Structure describing registers used by the ISR handler.
 */
struct registers_t
{
    uint32_t ds;                                        /*!< Data segment selector */
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;    /*!< Pushed by pusha */
    uint32_t int_no, err_code;                          /*!< Interrupt number and error code (if applicable) */
    uint32_t eip, cs, eflags, useresp, ss;              /*!< Pushed by the processor automatically */
};
//struct registers_t
//{
//    /*!< Pushed by pushal */
//    uint32_t edi, esi, ebp,
//            oesp,    // useless & ignored
//    ebx, edx, ecx, eax;
//
//    // rest of trap frame
//    uint16_t gs;
//    uint16_t padding1;
//    uint16_t fs;
//    uint16_t padding2;
//    uint16_t es;
//    uint16_t padding3;
//    uint16_t ds;
//    uint16_t padding4;
//    uint32_t int_no;                          /*!< Interrupt number */
//
//    // below here defined by x86 hardware
//    /*!< Pushed by the processor automatically */
//    uint32_t err_code;   // (if applicable)
//    uint32_t eip;
//    uint16_t cs;
//    uint16_t padding5;
//    uint32_t eflags;
//
//    // extra pushed by the processor only when crossing rings, such as from user to kernel
//    uint32_t esp;
//    uint16_t ss;
//    uint16_t padding6;
//};

typedef void (*isr_t)(struct registers_t *);

void register_interrupt_handler(uint8_t n, isr_t handler);

void isr_install(void);

void isr_handler(struct registers_t *regs);


#endif /* #define __ARCH_ISR_H__ */
