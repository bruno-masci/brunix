#include <asm/timer.h>
//#include <task.h>
#include <asm/isr.h>
#include <asm/irq.h>
#include <asm/io.h>
//#include <brunix/stdio.h>
#include <asm/processor.h>

#include <brunix/console.h>


#define WAIT_SOME_TIME() do { uint64_t start = rdtsc(); \
			      while(rdtsc() - start < 1000000) ; \
			} while (0)


#define LATCH(f)	((CLOCK_TICK_RATE + f/2) / f)
#define TIMER_FREQ		100	/* in HZ */
#define CLOCK_TICK_RATE		1193182	/* 8254 chip's internal oscillator frequency */



void sleep(uint64_t ticks);


/* This will keep track of how many ticks the system has been running for */
static volatile uint64_t timer_ticks = 0;


uint64_t get_clock_ticks(void) {
    return timer_ticks;
}

__attribute__((optimize("O0"))) void sleep(uint64_t ticks) {
    uint64_t start = rdtsc();
    while (rdtsc() - start < ticks)
        ;
}

void wait_some_time(void) {
    sleep(100000000);
}

static void timer_callback(__attribute__((unused)) struct registers_t *regs) {
    timer_ticks++;
    //TODO switch_task ();
//    printk("Timer interrupt: 0x%x (error code: %d)\n", regs->int_no, regs->err_code);
//    printk("CS: 0x%x; EIP: 0x%x; EFLAGS = %b; DS: 0x%x; EAX: 0x%x; USS: 0x%x \n", regs->cs, regs->eip, regs->eflags, regs->ds, regs->eax, regs->useresp);
}

/*
 * Sets up the system clock by installing the timer handler into IRQ0
 */
void timer_init(uint32_t frequency) {
    request_irq(IRQ0-32, timer_callback);

    // The value we send to the PIT is the value to divide it's input clock
    // (1193180 Hz) by, to get our required frequency. Important to note is
    // that the divisor must be small enough to fit into 16-bits.
    uint32_t divisor = 1193180 / frequency;

    // Send the command byte.
    outb(0x43, 0x36);

//    wait_some_time();
    WAIT_SOME_TIME();

    // Divisor has to be sent byte-wise, so split here into upper/lower bytes.
    uint8_t l = (uint8_t)(divisor & 0xFF);
    uint8_t h = (uint8_t)( (divisor>>8) & 0xFF );

    // Send the frequency divisor.
    outb(0x40, l);

//    wait_some_time();
    WAIT_SOME_TIME();

    outb(0x40, h);
}
