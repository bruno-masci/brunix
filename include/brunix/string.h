#ifndef __STRING_H2__
#define __STRING_H2__


#include <stddef.h>     // for size_t


int	strnlen(const char *str, size_t size);
char * strncpy(char *dst, const char *src, size_t size);

char * strfind(const char *s, char c);

int strstr(const char *in, const char *str);

#endif /* __STRING_H2__ */

#ifndef __STRING_H__
#define __STRING_H__


#include <stddef.h>     // for size_t

int	strcmp(const char *s1, const char *s2);

char * itoa(int value, char *str, int base);

const void * memset(const void *from_addr, int value, size_t count);

#endif /* __STRING_H__ */
