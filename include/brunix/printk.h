#include <asm/vga.h>

#define va_start	__builtin_va_start
#define va_list		__builtin_va_list
#define va_arg		__builtin_va_arg
#define va_end		__builtin_va_end


void printk(const char *fmt, ...);

#define debug(module_name, fmt, ...)		set_foreground_color(COLOR_LIGHT_GREY); printk("[" module_name "]: " fmt "\n", __VA_ARGS__); set_foreground_color(COLOR_WHITE);

#define debug_noargs(module_name, msg) 		set_foreground_color(COLOR_LIGHT_GREY); printk("[" module_name "]: " msg "\n"); set_foreground_color(COLOR_WHITE);

#define panic(fmt, ...)						set_foreground_color(COLOR_RED); printk("KERNEL PANIC: "fmt, __VA_ARGS__); while(1);

#define panic_noargs(msg) 					set_foreground_color(COLOR_RED); printk("KERNEL PANIC: "msg); while(1);
