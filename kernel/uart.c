// Intel 8250 serial port (UART).
// Based on https://github.com/guilleiguaran/xv6

#include <stdbool.h>

#include "../include/arch/x86/io.h"
#include "../include/arch/x86/uart.h"
#include "../include/brunix/stdio.h"

inline static void config_baud_rate(void) {
    // Set speed; requires DLAB latch
    outb(COM1+COM_LCR, COM_LCR_DLAB);
    outb(COM1+COM_DLL, (uint8_t) (115200 / 9600));
    outb(COM1+COM_DLM, 0);
}

inline static void config_line(void) {
    // 8 data bits, 1 stop bit, parity off; turn off DLAB latch
    outb(COM1+COM_LCR, COM_LCR_WLEN8 & ~COM_LCR_DLAB);
}

void uart_init(void) {
    info_noargs("uart", "Initializing serial port...");

    config_baud_rate();
    config_line();
}

inline static bool is_transmit_fifo_empty(void) {
    return inb(COM1 + COM_LSR) & COM_LSR_TXRDY;
}

void uart_putc(int c) {
    while (!is_transmit_fifo_empty())
        ;
    outb(COM1 + COM_TX, c);
}