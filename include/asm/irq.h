#ifndef __ARCH_IRQ_H__
#define __ARCH_IRQ_H__

#include <asm/traps.h>


#define NR_IRQS 16


#define FIRST_EXTERNAL_VECTOR		0x20


#define T_IRQ0          FIRST_EXTERNAL_VECTOR      // IRQ 0 corresponds to int T_IRQ

#define IRQ_TIMER        0
#define IRQ_KBD          1
#define IRQ_COM1         4
#define IRQ_IDE         14


#define PIC1			0x20		/* IO base address for master PIC */
#define PIC2			0xA0		/* IO base address for slave PIC */
#define PIC1_COMMAND	PIC1
#define PIC1_DATA		(PIC1+1)
#define PIC2_COMMAND	PIC2
#define PIC2_DATA		(PIC2+1)
#define PIC_EOI			0x20		/* End-of-interrupt command code */



void irq0(void);
void irq1(void);
void irq2(void);
void irq3(void);
void irq4(void);
void irq5(void);
void irq6(void);
void irq7(void);
void irq8(void);
void irq9(void);
void irq10(void);
void irq11(void);
void irq12(void);
void irq13(void);
void irq14(void);
void irq15(void);


/**
 * Allocates an interrupt and add a handler for an interrupt line.
 *
 * @irq_nr:     The interrupt line to allocate.
 * @handler:    Function to be called when the IRQ occurs.
 */
int request_irq(uint8_t irq_nr, trap_handler_t handler);

//extern const void *free_irq(unsigned int, void *);
//extern void enable_irq(unsigned int irq);
//extern void disable_irq(unsigned int irq);




#endif /* #define __ARCH_IRQ_H__ */
