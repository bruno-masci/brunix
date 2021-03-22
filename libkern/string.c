// Based on https://github.com/guilleiguaran/xv6
// Basic string routines.

#include <brunix/string.h>


int strnlen(const char *str, size_t size) {
	int n;

	for (n = 0; size > 0 && *str != '\0'; str++, size--)
		n++;

	return n;
}

char * strncpy(char *dst, const char *src, size_t size) {
    char *orig = dst;

    while (*src != '\0' && size > 0) {
        *dst++ = *src++;
        size--;
    }

    *dst = '\0';
    return orig;
}
