/**
 * @author Bruno Masci
 * @brief VGA output related code
 */

#ifndef __ARCH_VGA_H__
#define __ARCH_VGA_H__

#include <brunix/stddef.h>
#include <brunix/screen.h>


#define VIDEO_MEM_ADDR 0xB8000 // the video memory address


// Initializes VGA output and clears the screen
void vga_init();

// Clears the screen
void vga_clear();

// Sets text color
void vga_set_foreground_color(uint8_t colour);

// Prints a character
void vga_putc(unsigned char c);

// Prints a string
void vga_puts(char *str);

// Prints value as decimal
void vga_putdec(uint32_t n);

// Prints value as hexadecimal
void vga_puthex(uint32_t n);


#endif /* __ARCH_VGA_H__ */
