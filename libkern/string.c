// Based on https://github.com/guilleiguaran/xv6
// Basic string routines.

#include <brunix/string.h>
#include <brunix/console.h>


int strnlen(const char *s, size_t size) {
	int n;

	for (n = 0; size > 0 && *s != '\0'; s++, size--)
		n++;
	return n;
}

int strcmp(const char *p, const char *q) {
	while (*p && *p == *q)
		p++, q++;
	return (int) ((unsigned char) *p - (unsigned char) *q);
}

char * strcpy(char *dest, const char *src) {
    char *orig = dest;
    while((*dest++ = *src++) != '\0')
        ;
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

/* Extracted from http://wiki.osdev.org/Printing_To_Screen */
char * itoa(int value, char *str, int base) {
    char *rc;
    char *ptr;
    char *low;
    // Check for supported base.
    if (base < 2 || base > 36) {
        *str = '\0';
        return str;
    }
    rc = ptr = str;
    // Set '-' for negative decimals.
    if (value < 0 && base == 10) {
        *ptr++ = '-';
    }
    // Remember where the numbers start.
    low = ptr;
    // The actual conversion.
    do {
        // Modulo is negative for negative value. This trick makes abs() unnecessary.
        *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz"[35 + value % base];
        value /= base;
    } while (value);
    // Terminating the string.
    *ptr-- = '\0';
    // Invert the numbers.
    while (low < ptr) {
        char tmp = *low;
        *low++ = *ptr;
        *ptr-- = tmp;
    }
    return rc;
}

/**
 * Fills the first n bytes of the memory area pointed to by s
 * with the constant byte c.
 * @returns a pointer to the memory area v.
 */
const void * memset(const void *s, int c, size_t n) {
    char *p;
    int m;

    p = (void *) s;
    m = (int) n;    //TODO revisar casting
    while (--m >= 0)
        *p++ = (char) c;    //TODO revisar casting

    return s;
}