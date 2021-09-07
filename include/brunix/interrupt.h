#ifndef __INTERRUPT_H__
#define __INTERRUPT_H__

#define MAX_HANDLERS	256

/**
 * enum irqreturn
 * @IRQ_NONE		interrupt was not from this device or was not handled
 * @IRQ_HANDLED		interrupt was handled by this device
 */
enum irqreturn {
    IRQ_NONE,
    IRQ_HANDLED
};

typedef enum irqreturn irqreturn_t;


typedef irqreturn_t (*irq_handler_t)(int, void *);


/**
 * struct irqaction - per interrupt action descriptor
 * @handler:	interrupt handler function
 * @next:	pointer to the next irqaction for shared interrupts
 * @irq:	interrupt number
 * @flags:	flags (see IRQF_* above)
 * @thread_fn:	interrupt handler function for threaded interrupts
// * @thread:	thread pointer for threaded interrupts
 */
struct irqaction {
    irq_handler_t		handler;
    struct irqaction	*next;
    irq_handler_t		thread_fn;
//    struct task_struct	*thread;
    unsigned int		irq;
    unsigned int		flags;
};

extern irqreturn_t no_action(int cpl, void *dev_id);



/**
 * Allocates an interrupt and add a handler for an interrupt line.
 *
 * @irq_nr:     The interrupt line to allocate.
 * @handler:    Function to be called when the IRQ occurs. If NULL, the default primary handler is installed.
 */
int request_irq(uint8_t irq_nr, irq_handler_t handler);

extern int __must_check
request_nmi(unsigned int irq, irq_handler_t handler, unsigned long flags,
            const char *name, void *dev);


extern const void *free_irq(unsigned int, void *);

extern const void *free_nmi(unsigned int irq, void *dev_id);

bool irq_has_action(unsigned int irq);
extern void disable_irq(unsigned int irq);
extern void enable_irq(unsigned int irq);
extern bool irq_percpu_is_enabled(unsigned int irq);

extern void enable_nmi(unsigned int irq);


#endif /* #define __INTERRUPT_H__ */
