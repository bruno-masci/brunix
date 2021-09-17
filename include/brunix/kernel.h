/**
 * @author Bruno Masci
 * @brief Some general kernel functions
 */

#ifndef __KERNEL_H__
#define __KERNEL_H__


int printk(const char *fmt, ...);

#define panic(message) _panic(__FILE__, __LINE__, message)
void _panic(const char *file_name, int line_number, const char *message);

#define ASSERT(c) if(!(c)) { panic("Assertion '" #c "' failed"); }
//BUG_ON(c)

#endif /* __KERNEL_H__ */
