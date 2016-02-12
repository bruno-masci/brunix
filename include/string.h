#include <types.h>


// Copy len bytes from src to dest.
void memcpy(u8int *dest, const u8int *src, u32int len);

// Write len copies of val into dest.
void memset(u8int *dest, u8int val, u32int len);

// Compare two strings. Should return -1 if str1 < str2, 0 if they are equal or 1 otherwise.
int strcmp(char *str1, char *str2);

// Copy the NULL-terminated string src into dest, and return dest.
char *strcpy(char *dest, const char *src);

// Concatenate the NULL-terminated string src onto the end of dest, and return dest.
char *strcat(char *dest, const char *src);