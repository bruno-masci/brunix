/**
 * @author Bruno Masci
 * @brief Some general kernel functions
 */

#ifndef __KERNEL_H__
#define __KERNEL_H__


#include <brunix/defs.h>            // for NORET_FUNC


int printk(const char *fmt, ...);

#define panic(message) _panic(__FILE__, __LINE__, message)
NORET_FUNC void _panic(const char *file_name, int file_number, const char *message);


#endif /* __KERNEL_H__ */