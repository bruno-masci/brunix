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

void set_fg_color(uint8_t color);


#define info(module_name, fmt, ...)		    set_fg_color(COLOR_LIGHT_BLUE); cprintf(module_name ": "); set_fg_color(COLOR_WHITE); cprintf(fmt "\n", __VA_ARGS__);

#define info_noargs(module_name, msg) 		set_fg_color(COLOR_LIGHT_BLUE); cprintf(module_name ": "); set_fg_color(COLOR_WHITE); cprintf(msg "\n");

#define debug(module_name, fmt, ...)		set_fg_color(COLOR_LIGHT_BLUE); cprintf(module_name ": "); set_fg_color(COLOR_LIGHT_GREY); cprintf(fmt "\n", __VA_ARGS__); set_fg_color(COLOR_WHITE);

#define debug_noargs(module_name, msg) 		set_fg_color(COLOR_LIGHT_BLUE); cprintf(module_name ": "); set_fg_color(COLOR_LIGHT_GREY); cprintf(msg "\n"); set_fg_color(COLOR_WHITE);

#define warn(module_name, fmt, ...)		    set_fg_color(COLOR_LIGHT_BLUE); cprintf(module_name ": "); set_fg_color(COLOR_LIGHT_MAGENTA); cprintf("[WARN] " fmt "\n", __VA_ARGS__); set_fg_color(COLOR_WHITE);

#define warn_noargs(module_name, msg) 		set_fg_color(COLOR_LIGHT_BLUE); cprintf(module_name ": "); set_fg_color(COLOR_LIGHT_MAGENTA); cprintf("[WARN] " msg "\n"); set_fg_color(COLOR_WHITE);

#define panic(fmt, ...)						set_fg_color(COLOR_RED); cprintf("Kernel panic: " fmt " (" __FILE__ ")", __VA_ARGS__); halt();

#define panic_noargs(msg) 					set_fg_color(COLOR_RED); cprintf("Kernel panic: " msg " (%s:%d)\n", __FILE__, __LINE__); halt();




// kernel/console.c
void cputchar(int c);
//int	getchar(void);

// libkern/printfmt.c
void printfmt(void (*putch)(int, void*), void *putdat, const char *fmt, ...);
void vprintfmt(void (*putch)(int, void*), void *putdat, const char *fmt, va_list);
int	snprintf(char *str, int size, const char *fmt, ...);
int	vsnprintf(char *str, int size, const char *fmt, va_list);

// libkern/printf.c
int	cprintf(const char *fmt, ...);
int	vcprintf(const char *fmt, va_list);


#endif /* __STDIO_H__ */