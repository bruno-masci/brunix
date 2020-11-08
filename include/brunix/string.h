#ifndef __STRING_H__
#define __STRING_H__


#include <stddef.h>     // for size_t


int	strnlen(const char *str, size_t size);
char * strncpy(char *dst, const char *src, size_t size);

char *strfind(const char *s, char c);

int strstr(const char *in, const char *str);

#endif /* __STRING_H__ */
