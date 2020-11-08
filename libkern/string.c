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

// Return a pointer to the first occurrence of 'c' in 's',
// or a pointer to the string-ending null character if the string has no 'c'.
char *
strfind(const char *s, char c)
{
    for (; *s; s++)
        if (*s == c)
            break;
    return (char *) s;
}

int strstr(const char *in, const char *str) {
    while (*in != '\0' && *str != '\0') {
        if (*str++ != *in++)
            return 0;
    }
    if (*in != '\0')
        return 0;
    return 1;
}
