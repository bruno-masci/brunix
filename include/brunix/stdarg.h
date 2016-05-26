/**
 * @author Bruno Masci
 * @brief Definitions for variable argument lists
 */

#ifndef __STDARG_H__
#define __STDARG_H__

typedef __builtin_va_list va_list;

/// Initialize a variable argument list
#define va_start        __builtin_va_start

/// Retrieve next argument
#define va_arg          __builtin_va_arg

/// End using variable argument list
#define va_end          __builtin_va_end


#endif /* __STDARG_H__ */