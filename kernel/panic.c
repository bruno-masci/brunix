/*
 * This function is used through-out the kernel to indicate a major problem.
 */

#include <brunix/kernel.h>
//#include <linux/sched.h>
#include <stdarg.h>
#include <arch/x86/processor.h>
#include <brunix/console.h>

void __panic(const char *file_name, int file_line, const char *message) {
    set_fg_color(COLOR_RED);
    cprintf("\nKernel panic: ");
    set_fg_color(COLOR_LIGHT_GREY);
    cprintf(message);
    set_fg_color(COLOR_RED);
    cprintf("\nat [%s:%d]", file_name, file_line);
    set_fg_color(COLOR_LIGHT_CYAN);
    cprintf("\n\nSystem halted!\n\n");

    cli();
    halt();
}