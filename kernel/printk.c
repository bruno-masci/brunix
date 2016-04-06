#include <asm/vga.h>
#include <brunix/printk.h>


void printk(const char *fmt, ...) {
    va_list params;
    va_start(params, fmt);

    int arg = 0;
    while (*fmt != '\0') {
        if (*fmt == '%') {
            char op = *(++fmt);
            switch (op) {
                case 'c':
                    vga_putc(va_arg(params, char*));
                    break;
                case 'd':
                case 'i':
                    vga_putdec(va_arg(params, int));
                    break;
                case 'u':
                	vga_putdec(va_arg(params, uint32_t));
                	break;
                case 'x':
                case 'X':
                    vga_puthex(va_arg(params, uint32_t));
                    break;
                case 'b':
                case 'B':
                    vga_putbin(va_arg(params, uint32_t));
                    break;
                case 's':
                    vga_puts(va_arg(params, char*));
                    break;

                default:
                    //WARN("bad specifier");
                    vga_puts("bad specifier");
                    return;
            }
        }

        else {
            vga_putc(*fmt);
        }

        fmt++;
    }

    va_end(params);
}
