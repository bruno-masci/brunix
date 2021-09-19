#ifndef __ARCH_SEGMENT_H__
#define __ARCH_SEGMENT_H__


#define __KERNEL_CS_SELECTOR 0x08

#define __KERNEL_DS_SELECTOR 0x10


#ifndef __ASSEMBLER__

    typedef enum {
        // We don't use processor's rings 1 or 2
        PRIVILEGE_LEVEL_KERNEL = 0X0,
        PRIVILEGE_LEVEL_USER = 0X3
    } segment_privilege_level_enum;

#endif  /* !__ASSEMBLER__ */


#endif /* __ARCH_SEGMENT_H__ */
