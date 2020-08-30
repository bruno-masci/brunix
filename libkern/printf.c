// Extracted from https://github.com/guilleiguaran/xv6

// Simple implementation of cprintf console output for the kernel,
// based on printfmt() and the kernel console's cputchar().

#include <stdarg.h>

#include <brunix/console.h>     // for vprintfmt(), cputchar()
#include <brunix/defs.h>        // for PRIVATE, EXPORT


EXPORT int vcprintf(const char *fmt, va_list ap);


PRIVATE void putch(int ch, int *cnt) {
	cputchar(ch);
    (*cnt)++;
}

int vcprintf(const char *fmt, va_list ap) {
	int cnt = 0;

	vprintfmt((void (*) (int, void *))putch, &cnt, fmt, ap);
	return cnt;
}