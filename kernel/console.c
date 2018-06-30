#include "../include/arch/x86/uart.h"
#include "../include/arch/x86/vga.h"

// output a character to the console
static void cons_putc(int c) {
	uart_putc(c);
	vga_putc(c);
}

// `High'-level console I/O.  Used by readline and cprintf.

void cputchar(int c) {
	cons_putc(c);
}