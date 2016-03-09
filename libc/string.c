#include <string.h>


// Copy len bytes from src to dest.
void memcpy(uint8_t *dest, const uint8_t *src, uint32_t len) {
    // TODO: implement this yourself!
}

// Write len copies of val into dest.
// from http://clc-wiki.net/wiki/C_standard_library:string.h:memset
void memset(uint8_t *dest, uint8_t val, uint32_t len) {
    uint8_t *p = dest;
    while(len--) {
        *p++ = (uint8_t) val;
    }
}

// Compare two strings. Should return -1 if str1 < str2, 0 if they are equal or 1 otherwise.
int strcmp(char *str1, char *str2) {
    // TODO: implement this yourself!
}

// Copy the NULL-terminated string src into dest, and return dest.
char *strcpy(char *dest, const char *src) {
    // TODO: implement this yourself!
}

// Concatenate the NULL-terminated string src onto the end of dest, and return dest.
char *strcat(char *dest, const char *src) {
    // TODO: implement this yourself!
}
