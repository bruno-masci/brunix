#ifndef __TIMER_H__
#define __TIMER_H__


#include <stdint.h>

uint64_t get_clock_ticks(void);

void timer_init(uint32_t frequency);

void wait_some_time(void);

#endif /* __TIMER_H__ */
