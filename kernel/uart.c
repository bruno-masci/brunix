// Intel 8250 serial port (UART).
// Based on https://github.com/guilleiguaran/xv6

#include <stdbool.h>        // for bool

#include <asm/uart.h>
#include <asm/io.h>

#include <brunix/console.h>
#include <brunix/defs.h>            // for PRIVATE


void uart_init(void);



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

// Initializes serial device
void uart_init(void) {
//    info_noargs("Initializing serial port...");

    config_baud_rate();
    config_line();

    //TODO ver esto   ioapicenable(IRQ_COM1, 0);
    // por qué funciona SIN interrupts?
}

//TODO validar si esto se usa!
PRIVATE inline bool is_transmit_fifo_empty(void) {
    return inb(COM1 + COM_LSR) & COM_LSR_TXRDY;
}

void uart_putc(char c) {
    while (!is_transmit_fifo_empty())
        ;
    outb(COM1 + COM_TX, (uint8_t) c);
}