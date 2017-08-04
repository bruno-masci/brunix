/**
 * @author Bruno Masci
 * @brief Stream related functions.
 */

#ifndef __STDIO_H__
#define __STDIO_H__

#include <stdarg.h>
#include "screen.h"
#include "../arch/x86/processor.h"


void video_init(void);

void printk(const char *fmt, ...);

void set_fg_color(uint8_t color);

#define debug(module_name, fmt, ...)		set_fg_color(COLOR_LIGHT_GREY); printk("[" module_name "]: " fmt "\n", __VA_ARGS__); set_fg_color(COLOR_WHITE);

#define debug_noargs(module_name, msg) 		set_fg_color(COLOR_LIGHT_GREY); printk("[" module_name "]: " msg "\n"); set_fg_color(COLOR_WHITE);

#define panic(fmt, ...)						set_fg_color(COLOR_RED); printk("Kernel panic: " fmt " (" __FILE__ ")", __VA_ARGS__); halt();

#define panic_noargs(msg) 					set_fg_color(COLOR_RED); printk("Kernel panic: " msg " (%s:%d)\n", __FILE__, __LINE__); halt();


#endif /* __STDIO_H__ */