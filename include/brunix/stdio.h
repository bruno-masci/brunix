/**
 * @author Bruno Masci
 * @brief Stream related functions.
 */

#ifndef __STDIO_H__
#define __STDIO_H__

#include <brunix/screen.h>
#include <brunix/stdarg.h>
#include <brunix/processor.h>


void video_init(void);

void printk(const char *fmt, ...);

void set_fg_color(uint8_t colour);

#define debug(module_name, fmt, ...)		set_fg_color(COLOR_LIGHT_GREY); printk("[" module_name "]: " fmt "\n", __VA_ARGS__); set_fg_color(COLOR_WHITE);

#define debug_noargs(module_name, msg) 		set_fg_color(COLOR_LIGHT_GREY); printk("[" module_name "]: " msg "\n"); set_fg_color(COLOR_WHITE);

#define panic(fmt, ...)						set_fg_color(COLOR_RED); printk("Kernel panic: " fmt " (" __FILE__ ")", __VA_ARGS__); while(1) { HALT; };

#define panic_noargs(msg) 					set_fg_color(COLOR_RED); printk("Kernel panic: " msg " (%s:%d)", __FILE__, __LINE__); while(1) { HALT; };


#endif /* __STDIO_H__ */
