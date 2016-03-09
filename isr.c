//
// isr.c -- High level interrupt service routines and interrupt request handlers.
// Part of this code is modified from Bran's kernel development tutorials.
// Rewritten for JamesM's kernel development tutorials.
//

#include "isr.h"
#include "arch/x86/kernel/vga.h"

// This gets called from our ASM interrupt handler stub.
void isr_handler(registers_t regs) {
   vga_puts("*** Received interrupt: ");
   vga_putdec(regs.int_no);
   vga_putdec(regs.err_code);
   vga_putc('\n');
}
