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

char *strcpy(char *dest, const char *src) {
    int i = 0;
    char *orig = dest;
    while (src[i] != '\0') {
        dest[i] = src[i];
        i++;
    }
    return orig;
}
char *strcpy3(char *dest, const char *src) {
    char *orig = dest;
    while((*dest++ = *src++)!= '\0')
        ; // <<== Very important!!!
    return orig;
}
char * strcpy1(char *destination, const char *source) {
    while (*source != '\0') {
        cprintf("CHAR1 %c\n", *source);
//        cprintf("CHAR2 %c\n", *destination);
        *destination++ = *source++;
    }
    return destination;
}
char *strcpy2(char *s1, const char *s2)
{
    char *s1_p = s1;
    while (*s1++ = *s2++)
        ;
    return s1_p;
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

const void * memset(const void *v, int c, size_t n) {
    char *p;
    int m;

    p = (void *) v;
    m = n;
    while (--m >= 0)
        *p++ = c;

    return v;
}