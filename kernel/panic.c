/*
 * This function is used throughout the kernel to indicate a major problem.
 */

#include <stdbool.h>                // for true

#include <brunix/kernel.h>
#include <arch/x86/processor.h>     // for cli(), halt()
#include <brunix/console.h>

void _panic(const char *file_name, int line_number, const char *message) {
    set_fg_color(COLOR_RED);
    printk("\nKernel panic: ");
    set_fg_color(COLOR_LIGHT_GREY);
    printk(message);
    set_fg_color(COLOR_RED);
    printk("\nat [%s:%d]", file_name, line_number);
    set_fg_color(COLOR_LIGHT_CYAN);
    printk("\n\nSystem halted!\n\n");

    // disable (maskable) interrupts
    cli();

    // Halt the CPU, and keep doing it in case some NMI event is generated
    do {
        halt();
    } while (true);
}