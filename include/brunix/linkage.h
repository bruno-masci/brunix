#ifndef _LINUX_LINKAGE_H
#define _LINUX_LINKAGE_H

//#include <linux/compiler.h>
#include <asm/linkage.h>

#define CPP_ASMLINKAGE

#ifndef asmlinkage
#define asmlinkage CPP_ASMLINKAGE
#endif

/*
 * This is used by architectures to keep arguments on the stack
 * untouched by the compiler by keeping them live until the end.
 * The argument stack may be owned by the assembly-language
 * caller, not the callee, and gcc doesn't always understand
 * that.
 *
 * We have the return value, and a maximum of six arguments.
 *
 * This should always be followed by a "return ret" for the
 * protection to work (ie no more work that the compiler might
 * end up needing stack temporaries for).
 */
/* Assembly files may be compiled with -traditional .. */
#ifndef __ASSEMBLY__
#ifndef asmlinkage_protect
# define asmlinkage_protect(n, ret, args...)	do { } while (0)
#endif
#endif

#ifndef __ALIGN
#define __ALIGN		.align 4,0x90
#define __ALIGN_STR	".align 4,0x90"
#endif

#ifdef __ASSEMBLY__

#ifndef LINKER_SCRIPT
#define ALIGN __ALIGN
#define ALIGN_STR __ALIGN_STR

#endif /* LINKER_SCRIPT */

#ifndef WEAK
#define WEAK(name)	   \
	.weak name;	   \
	name:
#endif

#ifndef END
#define END(name) \
  .size name, .-name
#endif

/* If symbol 'name' is treated as a subroutine (gets called, and returns)
 * then please use ENDPROC to mark 'name' as STT_FUNC for the benefit of
 * static analysis tools such as stack depth analyzer.
 */
#ifndef ENDPROC
#define ENDPROC(name) \
  .type name, @function; \
  END(name)
#endif

#endif

#endif
