#include "timer.h"
//#include <task.h>
#include <asm/isr.h>
#include <asm/irq.h>
#include <asm/io.h>
#include <brunix/printk.h>



inline static uint64_t rdtsc(void)//TODO mover a processor.h. ver eduOS
{
	uint64_t x;
	asm volatile ("rdtsc" : "=A" (x));
	return x;
}
#define WAIT_SOME_TIME() do { uint64_t start = rdtsc(); \
			      while(rdtsc() - start < 1000000) ; \
			} while (0)
#define LATCH(f)	((CLOCK_TICK_RATE + f/2) / f)
#define TIMER_FREQ		100	/* in HZ */
#define CLOCK_TICK_RATE		1193182	/* 8254 chip's internal oscillator frequency */





/* This will keep track of how many ticks the system has been running for */
static volatile uint64_t timer_ticks = 0;


uint64_t get_clock_tick(void) {
	return timer_ticks;
}

static void timer_callback(struct registers_t *regs) {
	timer_ticks++;
    //TODO switch_task ();
	//debug("timer", "Int. no: %d; Error code: %d; CS: %x; EIP: %x; EFLAGS = %b", regs->int_no, regs->err_code, regs->cs, regs->eip, regs->eflags);
    //printk ("Tick: %d\n", timer_ticks);
}


/*
 * Sets up the system clock by installing the timer handler into IRQ0
 */
void init_timer(uint32_t frequency) {
	register_irq_handler(IRQ0, &timer_callback);

    // The value we send to the PIT is the value to divide it's input clock
    // (1193180 Hz) by, to get our required frequency. Important to note is
    // that the divisor must be small enough to fit into 16-bits.
    uint32_t divisor = 1193180 / frequency;

    // Send the command byte.
    outb(0x43, 0x36);

    WAIT_SOME_TIME();

    // Divisor has to be sent byte-wise, so split here into upper/lower bytes.
    uint8_t l = (uint8_t)(divisor & 0xFF);
    uint8_t h = (uint8_t)( (divisor>>8) & 0xFF );

    // Send the frequency divisor.
    outb(0x40, l);

    WAIT_SOME_TIME();

    outb(0x40, h);
}
