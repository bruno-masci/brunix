// Extracted from https://github.com/guilleiguaran/xv6

// Simple implementation of cprintf console output for the kernel,
// based on printfmt() and the kernel console's cputchar().

#include <stdarg.h>

#include <brunix/console.h>     // for vprintfmt() and cputchar()
#include <brunix/defs.h>        // for PRIVATE


PRIVATE int vcprintf(const char *fmt, va_list ap);


PRIVATE void putch(int ch, int *cnt) {
	cputchar(ch);
    (*cnt)++;
}

PRIVATE int vcprintf(const char *fmt, va_list ap) {
	int cnt = 0;

	vprintfmt((void (*) (int, void *))putch, &cnt, fmt, ap);
	return cnt;
}

int cprintf(const char *fmt, ...) {
	va_list ap;
	int cnt;

	va_start(ap, fmt);
	cnt = vcprintf(fmt, ap);
	va_end(ap);

	return cnt;
}