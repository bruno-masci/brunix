/**
 * @author Bruno Masci
 * @brief Some general kernel functions
 */

#ifndef __KERNEL_H__
#define __KERNEL_H__


void panic(const char *fmt, ...) __attribute__ ((noreturn));

//TODO no se usa aun
#define ASSERT(c) if(!(c)) { panic("%s:%d: assert %s failed", __FILE__, __LINE__, #c); }


#endif /* __KERNEL_H__ */