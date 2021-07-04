/*
 * This function is used throughout the kernel to indicate a major problem.
 */

#include <stdbool.h>                // for true

#include <brunix/kernel.h>
#include <asm/processor.h>     // for cli(), halt()
#include <brunix/console.h>
#include <brunix/kdebug.h>

void _panic(const char *file_name, int line_number, const char *message) {
    set_fg_color(COLOR_LIGHT_GREY);
    printk("\nKernel panic: ");
    set_fg_color(COLOR_RED);
    printk(message);
    set_fg_color(COLOR_LIGHT_GREY);
    printk("\n[at %s:%d]", file_name, line_number);

    set_fg_color(COLOR_LIGHT_GREY);
    print_stack_backtrace(false);

    set_fg_color(COLOR_LIGHT_CYAN);
    printk("\n\nSystem halted!\n\n");

    // disable (maskable) interrupts
    cli();

    // Halt the CPU, and keep doing it in case some NMI event is generated
    do {
        halt();
    } while (true);
}
