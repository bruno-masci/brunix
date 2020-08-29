// Based on https://github.com/guilleiguaran/xv6
// Basic string routines.

#include <brunix/string.h>


int strnlen(const char *str, size_t size) {
	int n;

	for (n = 0; size > 0 && *str != '\0'; str++, size--)
		n++;
	return n;
}

char * strcpy(char *dst, const char *src) {
    char *orig = dst;
    while((*dst++ = *src++) != '\0')
        ;
    return orig;
}