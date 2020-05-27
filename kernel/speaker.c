#include <arch/x86/timer.h>
//#include <task.h>
#include <arch/x86/isr.h>
#include <arch/x86/irq.h>
#include <arch/x86/io.h>
#include <arch/x86/processor.h>

#include <brunix/console.h>



#define WAIT_SOME_TIME() do { uint64_t start = rdtsc(); \
			      while(rdtsc() - start < 1000000) ; \
			} while (0)
#define LATCH(f)	((CLOCK_TICK_RATE + f/2) / f)
#define TIMER_FREQ		100	/* in HZ */
#define CLOCK_TICK_RATE		1193182	/* 8254 chip's internal oscillator frequency */


volatile void wait_some_time() {
    uint64_t start = rdtsc();
	while (rdtsc() - start < 100000000)
	    ;
}



////
//Play sound using built in speaker
void play_sound(uint32_t nFrequence) {
    uint32_t Div;
    uint8_t tmp;

    //Set the PIT to the desired frequency
    Div = 1193180 / nFrequence;
    outb(0x43, 0xb6);
    outb(0x42, (uint8_t) (Div) );
    outb(0x42, (uint8_t) (Div >> 8));

    //And play the sound using the PC speaker
    tmp = inb(0x61);
    if (tmp != (tmp | 3)) {
        outb(0x61, tmp | 3);
    }
}

//make it shutup
void nosound() {
    uint8_t tmp = inb(0x61) & ~3;
    outb(0x61, tmp);
}

//Make a beep
void beep() {
    cprintf("B");
    play_sound(1000);
//    timer_wait(10);
//    for(int i=0;i<9999;i++);
    wait_some_time();
    cprintf("C");
    nosound();
    //set_PIT_2(old_frequency);
}