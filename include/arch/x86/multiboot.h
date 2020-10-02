/**
 * @author Bruno Masci
 * @brief Declarations related to the Multiboot Standard interface.
 * @see /kernel/multiboot_entry_point.S
 * @see http://wiki.osdev.org/Multiboot
 */

#ifndef __ARCH_MULTIBOOT_H__
#define __ARCH_MULTIBOOT_H__


#define MBOOT_HEADER_MAGIC      0x1BADB002
#define MBOOT_HEADER_FLAGS      0x00000000
#define MBOOT_HEADER_CHECKSUM   -(MBOOT_HEADER_MAGIC + MBOOT_HEADER_FLAGS)


#endif /* __ARCH_MULTIBOOT_H__ */
