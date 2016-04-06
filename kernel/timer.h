#ifndef __TIMER_H__
#define __TIMER_H__


#include <brunix/stddef.h>

uint64_t get_clock_tick(void);

void init_timer (uint32_t frequency);


#endif /* __TIMER_H__ */
