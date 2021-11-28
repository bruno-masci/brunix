/**
 * @author Bruno Masci
 * @brief VGA output related code
 */

#ifndef __ARCH_VGA_H__
#define __ARCH_VGA_H__


#include <stdint.h>         // for uint8_t
#include <brunix/defs.h>    // for INIT_FUNC


#define COLOR_BLACK 			0x00
#define COLOR_BLUE 				0x01
#define COLOR_GREEN 			0x02
#define COLOR_CYAN 				0x03
#define COLOR_RED 				0x04
#define COLOR_MAGENTA 			0x05
#define COLOR_BROWN 			0x06
#define COLOR_LIGHT_GREY 		0x07
#define COLOR_DARK_GREY 		0x08
#define COLOR_LIGHT_BLUE 		0x09
#define COLOR_LIGHT_GREEN 		0x0A
#define COLOR_LIGHT_CYAN 		0x0B
#define COLOR_LIGHT_RED 		0x0C
#define COLOR_LIGHT_MAGENTA 	0x0D
#define COLOR_LIGHT_BROWN 		0x0E
#define COLOR_WHITE 			0x0F


#define VIDEO_MEM_ADDR 0xB8000 // the video start memory address


// Sets text color
void vga_set_foreground_color(uint8_t colour);

// Clears the screen
void vga_clear(void);

// Prints a character
void vga_putc(char c);


#endif /* __ARCH_VGA_H__ */
