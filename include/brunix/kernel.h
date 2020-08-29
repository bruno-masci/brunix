/**
 * @author Bruno Masci
 * @brief Some general kernel functions
 */

#ifndef __KERNEL_H__
#define __KERNEL_H__


#define panic(message) __panic(__FILE__, __LINE__, message)


void __panic(const char *file_name, int file_number, const char *message) __attribute__ ((noreturn));


#endif /* __KERNEL_H__ */