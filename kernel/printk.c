#include <stdarg.h>
#include <brunix/kernel.h>


extern int vcprintf(const char *fmt, va_list args);


int printk(const char *fmt, ...) {
    va_list ap;
    int cnt;

    va_start(ap, fmt);
    cnt = vcprintf(fmt, ap);
    va_end(ap);

    return cnt;
}