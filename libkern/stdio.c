#include "../include/arch/x86/vga.h"
#include "../include/brunix/stdio.h"


void video_init(void) {
    vga_init();
}

void set_fg_color(uint8_t color) {
    vga_set_foreground_color(color);
}