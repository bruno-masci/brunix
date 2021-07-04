/**
 * @author Bruno Masci
 * @brief Console management
 */

#ifndef __CONSOLE_H__
#define __CONSOLE_H__


#include <stdint.h>                 // for uint8_t
#include <asm/vga.h>
#include <asm/uart.h>


void cputchar(char c);
void set_fg_color(uint8_t color);


#endif /* __CONSOLE_H__ */
