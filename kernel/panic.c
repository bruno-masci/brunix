/*
 * This function is used through-out the kernel to indicate a major problem.
 */

#include <brunix/kernel.h>
//#include <linux/sched.h>
#include <stdarg.h>
#include <arch/x86/processor.h>
#include <brunix/console.h>

void panic(const char *fmt, ...) {
    set_fg_color(COLOR_RED);

    va_list args;
    va_start(args, fmt);
    cprintf("Kernel panic (%s:%d): ", __FILE__, __LINE__);
    cprintf(fmt, args);
    cprintf("\nSystem halted!");
    va_end(args);

    //	if (current == task[0])
//		cprintf("In swapper task - not syncing\n\r");
//	else
//		sys_sync();

//    while (true);

    cli();
    halt();
}
