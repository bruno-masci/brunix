// Intel 8250 serial port (UART).
// Based on https://github.com/guilleiguaran/xv6

#include <stdbool.h>        // for bool

#include <arch/x86/io.h>
#include <arch/x86/uart.h>

#include <brunix/console.h>
#include <brunix/defs.h>            // for PRIVATE

PRIVATE inline void config_baud_rate(void) {
    // Set speed; requires DLAB latch
    outb(COM1+COM_LCR, COM_LCR_DLAB);
    outb(COM1+COM_DLL, (uint8_t) (115200 / 9600));
    outb(COM1+COM_DLM, 0);
}

PRIVATE inline void config_line(void) {
    // 8 data bits, 1 stop bit, parity off; turn off DLAB latch
    outb(COM1+COM_LCR, COM_LCR_WLEN8 & ~COM_LCR_DLAB);
}

void uart__init(void) {
    info_noargs("Initializing serial port...");

    config_baud_rate();
    config_line();
}

//TODO validar si esto se usa!
PRIVATE inline bool is_transmit_fifo_empty(void) {
    return inb(COM1 + COM_LSR) & COM_LSR_TXRDY;
}

void uart__putc(int c) {
    while (!is_transmit_fifo_empty())
        ;
    outb(COM1 + COM_TX, c);
}