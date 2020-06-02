#ifndef __TIMER_H__
#define __TIMER_H__


#include <stdint.h>

uint64_t get_clock_ticks(void);

void sleep(uint64_t ticks);

void wait_some_time(void);


void timer_init(uint32_t frequency);


#endif /* __TIMER_H__ */