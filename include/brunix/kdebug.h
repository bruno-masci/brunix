#ifndef JOS_KERN_KDEBUG_H
#define JOS_KERN_KDEBUG_H

#include <stdint.h>
#include <stdbool.h>

struct FunctionParam {
    char param_name[64];	// Source code filename for EIP
    bool is_ptr;			// Source code linenumber for EIP
};

// Debug information about a particular instruction pointer
struct Eipdebuginfo {

    // Please note there's no allocated space for these (char *) because they will point to a stab in the kernel image

    uintptr_t eip;		// Current Address
    uint32_t ebp;		// Current EBP

    char eip_file[100];		// Source code filename for EIP
    int eip_line;			// Source code linenumber for EIP

    char eip_fn_name[50];	// Name of function containing EIP
    //  - Note: not null terminated!
    uintptr_t eip_fn_addr;		// Address of start of function
    int eip_fn_narg;		// Number of function arguments

    int valid;
    struct FunctionParam fn_params[10];
};

int stack_backtrace(struct Eipdebuginfo info[]);
void print_stack_backtrace(bool ptr_as_string);

#endif
