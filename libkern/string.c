#include <brunix/string.h>

void* memcpy(void *dest, const void *src, size_t n) {
    // TODO: implement
}

// from http://clc-wiki.net/wiki/C_standard_library:string.h:memset
void* memset(void *dest, int val, size_t n) {
    char *p = dest;
    while(n--) {
        *p++ = (char) val;
    }

    return dest;
}

int strcmp(const char *str1, const char *str2) {
    // TODO: implement
}

char *strcpy(char *dest, const char *src) {
    // TODO: implement
}

char *strcat(char *dest, const char *src) {
    // TODO: implement
}