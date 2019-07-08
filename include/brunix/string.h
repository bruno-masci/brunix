#ifndef __STRING_H__
#define __STRING_H__


#include <stddef.h>     // for size_t

int	strnlen(const char *s, size_t size);
int	strcmp(const char *s1, const char *s2);

char * itoa(int value, char *str, int base);


#endif /* __STRING_H__ */