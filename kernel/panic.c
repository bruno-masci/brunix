/*
 * This function is used through-out the kernel to indicate a major problem.
 */

#include <brunix/kernel.h>
//#include <linux/sched.h>
#include <arch/x86/processor.h>
#include <brunix/console.h>

void _panic(const char *file_name, int file_line, const char *message) {
    set_fg_color(COLOR_RED);
    printk("\nKernel panic: ");
    set_fg_color(COLOR_LIGHT_GREY);
    printk(message);
    set_fg_color(COLOR_RED);
    printk("\nat [%s:%d]", file_name, file_line);
    set_fg_color(COLOR_LIGHT_CYAN);
    printk("\n\nSystem halted!\n\n");

    cli();

    // Halts the CPU, and keeps doing it in case some NMI event is generated
    do {
        halt();
    } while (true);
}