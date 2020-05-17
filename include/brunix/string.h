#ifndef __STRING_H__
#define __STRING_H__


#include <stddef.h>     // for size_t

int	strnlen(const char *s, size_t size);
int	strcmp(const char *s1, const char *s2);
char * strcpy(char *destination, const char *source);

char * itoa(int value, char *str, int base);

const void * memset(const void *from_addr, int value, size_t count);

#endif /* __STRING_H__ */