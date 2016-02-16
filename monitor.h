#ifndef MONITOR_H
#define MONITOR_H

#include <stdint.h>


#define COLOR_BLACK 0x00
#define COLOR_BLUE 0x01
#define COLOR_GREEN 0x02
#define COLOR_CYAN 0x03
#define COLOR_RED 0x04
#define COLOR_MAGENTA 0x05
#define COLOR_BROWN 0x06
#define COLOR_LIGHT_GREY 0x07
#define COLOR_DARK_GREY 0x08
#define COLOR_LIGHT_BLUE 0x09
#define COLOR_LIGHT_GREEN 0x0A
#define COLOR_LIGHT_CYAN 0x0B
#define COLOR_LIGHT_RED 0x0C
#define COLOR_LIGHT_MAGENTA 0x0D
#define COLOR_LIGHT_BROWN 0x0E
#define COLOR_WHITE 0x0F


// Clear the screen to all black.
void monitor_clear();

// Write a single character out to the screen.
void monitor_put(char c);

// Output a null-terminated ASCII string to the monitor.
void monitor_write(char *c);

void monitor_write_dec(uint32_t n);

void monitor_write_hex(uint32_t n);


#endif // MONITOR_H
