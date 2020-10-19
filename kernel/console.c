#include <brunix/console.h>
#include <brunix/defs.h>            // for PRIVATE, INIT_FUNC


INIT_FUNC void console_init(void);
PRIVATE void console_putc(int c);


#define ORIG_VIDEO_COLS 	80
#define ORIG_VIDEO_LINES	25
#define VIDEO_SIZE_ROW      (ORIG_VIDEO_COLS * 2)
#define SCREEN_SIZE         (ORIG_VIDEO_LINES * VIDEO_SIZE_ROW);        // 4k

// We send all the output to both the screen and the serial port (emulator)

INIT_FUNC void console_init(void) {
    vga_init();
    uart_init();
}

// `High'-level console I/O.  Used by readline and cprintf.
void cputchar(int c) {
    console_putc(c);
}

void set_fg_color(uint8_t color) {
    vga_set_foreground_color(color);
}

// output a character to the console
PRIVATE void console_putc(int c) {
    vga_putc(c);
    uart_putc(c);
}