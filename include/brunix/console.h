/**
 * @author Bruno Masci
 * @brief Stream related functions.
 */

#ifndef __CONSOLE_H__
#define __CONSOLE_H__


#include <stdarg.h>                 // for va_list
#include <arch/x86/processor.h>
#include <arch/x86/vga.h>


// kernel/console.c
void cputchar(int c);
void set_fg_color(uint8_t color);

// libkern/printfmt.c
void vprintfmt(void (*putch)(int, void*), void *putdat, const char *fmt, va_list);


#endif /* __CONSOLE_H__ */


//void set_fg_color(uint8_t color);

/* vga.c */
//void vga__init(void);
//void cputchar(int c);