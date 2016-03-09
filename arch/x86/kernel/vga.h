#ifndef __ARCH_VGA_H__
#define __ARCH_VGA_H__

#include <stdint.h>


#define COLOR_BLACK 		0x00
#define COLOR_BLUE 		0x01
#define COLOR_GREEN 		0x02
#define COLOR_CYAN 		0x03
#define COLOR_RED 		0x04
#define COLOR_MAGENTA 		0x05
#define COLOR_BROWN 		0x06
#define COLOR_LIGHT_GREY 	0x07
#define COLOR_DARK_GREY 	0x08
#define COLOR_LIGHT_BLUE 	0x09
#define COLOR_LIGHT_GREEN 	0x0A
#define COLOR_LIGHT_CYAN 	0x0B
#define COLOR_LIGHT_RED 	0x0C
#define COLOR_LIGHT_MAGENTA 	0x0D
#define COLOR_LIGHT_BROWN 	0x0E
#define COLOR_WHITE 		0x0F


// Initializes VGA output and clears the screen
void vga_init();

// Clears the screen
void vga_clear();

// Prints a character
void vga_putc(unsigned char c);

// Prints a string
void vga_puts(char *str);

// Prints value as decimal
void vga_putdec(uint32_t n);

// Prints value as hexadecimal
void vga_puthex(uint32_t n);


#endif // __ARCH_VGA_H__
