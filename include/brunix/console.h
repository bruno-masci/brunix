/**
 * @author Bruno Masci
 * @brief Stream related functions.
 */

#ifndef __CONSOLE_H__
#define __CONSOLE_H__


#include <stdarg.h>                 // for va_list
#include <arch/x86/processor.h>
#include <arch/x86/vga.h>


void cputchar(int c);
void set_fg_color(uint8_t color);


#endif /* __CONSOLE_H__ */