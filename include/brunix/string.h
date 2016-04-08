/**
 * @author Bruno Masci
 * @brief Definition of basic string and memory operations.
 *
 * This file deals with memcpy, memset, string functions and everything related to continuous byte fields.
 */

#ifndef __STRING_H__
#define __STRING_H__

#include <brunix/stddef.h>

// Copy n bytes from src to dest.
void* memcpy(void *dest, const void *src, size_t n);

// Set n bytes of dest to val.
void* memset(void *dest, int val, size_t n);

// Compare two strings. Should return -1 if str1 < str2, 0 if they are equal or 1 otherwise.
int strcmp(const char *str1, const char *str2);

// Copy the NULL-terminated string src into dest, and return dest.
char* strcpy(char *dest, const char *src);

// Concatenate the NULL-terminated string src onto the end of dest, and return dest.
char* strcat(char *dest, const char *src);


//size_t strlen(const char *str);
//char* strncpy(char *dest, const char *src, size_t n);
//int strncmp(const char *s1, const char *s2, size_t n);


#endif /* __STRING_H__ */
