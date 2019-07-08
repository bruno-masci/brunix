/**
 * @author Bruno Masci
 * @brief Stream related functions.
 */

#ifndef __CONSOLE_H__
#define __CONSOLE_H__


#include <stdarg.h>                 // for va_list
#include <arch/x86/processor.h>
#include <arch/x86/vga.h>


#define info(fmt, ...)		        set_fg_color(COLOR_WHITE); \
                                    cprintf(fmt "\n", __VA_ARGS__);

#define info_noargs(msg) 		    set_fg_color(COLOR_WHITE); \
                                    cprintf(msg "\n");

#define debug(fmt, ...)		        set_fg_color(COLOR_LIGHT_GREY); \
                                    cprintf("<debug> " fmt "\n", __VA_ARGS__); \
                                    set_fg_color(COLOR_WHITE);

#define debug_noargs(msg)           set_fg_color(COLOR_LIGHT_GREY); \
                                    cprintf("<debug> " msg "\n"); \
                                    set_fg_color(COLOR_WHITE);

#define warn(fmt, ...)              set_fg_color(COLOR_LIGHT_MAGENTA); \
                                    cprintf("<warn> " fmt "\n", __VA_ARGS__); \
                                    set_fg_color(COLOR_WHITE);

#define warn_noargs(msg)            set_fg_color(COLOR_LIGHT_MAGENTA); \
                                    cprintf("<warn> " msg "\n"); \
                                    set_fg_color(COLOR_WHITE);


int cprintf(const char *fmt, ...);

// kernel/console.c
void cputchar(int c);
void set_fg_color(uint8_t color);

// libkern/printfmt.c
void printfmt(void (*putch)(int, void*), void *putdat, const char *fmt, ...);
void vprintfmt(void (*putch)(int, void*), void *putdat, const char *fmt, va_list);



/* uart.c */
void uart__init(void);
void uart__putc(int c);

#endif /* __CONSOLE_H__ */


//void set_fg_color(uint8_t color);

/* vga.c */
//void vga__init(void);
//void cputchar(int c);