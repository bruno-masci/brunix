/**
 * @author Bruno Masci
 * @brief Intel 8250 serial port (UART). Based on https://github.com/guilleiguaran/xv6
 * @see https://wiki.osdev.org/Serial_Ports
 */

#ifndef __ARCH_UART_H__
#define __ARCH_UART_H__


#include <brunix/defs.h>    // for INIT_FUNC

/***** Serial I/O code *****/

#define COM1		0x3F8

#define COM_TX		    0	    // Out: Transmit buffer (DLAB=0)
#define COM_DLL		    0	    // Out: Divisor Latch Low (DLAB=1)
#define COM_DLM		    1	    // Out: Divisor Latch High (DLAB=1)
#define COM_LCR		    3	    // Out: Line Control Register
#define COM_LCR_DLAB	0x80	//   Divisor latch access bit
#define COM_LCR_WLEN8	0x03	//   Wordlength: 8 bits
#define COM_LSR		    5	    // In: Line Status Register
#define COM_LSR_TXRDY	0x20	//   Transmit buffer avail


// Sends a character through the first serial line (akka ttyS0)
void uart_putc(char c);


#endif /* !__ARCH_UART_H__ */
