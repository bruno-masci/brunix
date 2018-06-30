/**
 * @author Bruno Masci
 * @brief Declarations related to the Multiboot Standard interface.
 * @see https://www.gnu.org/software/grub/manual/multiboot/multiboot.html
 */

#ifndef __ARCH_MULTIBOOT_H__
#define __ARCH_MULTIBOOT_H__


#include <stddef.h>

#define MULTIBOOT_HEADER_MAGIC 0x2BADB002

struct multiboot_info {
    uint32_t flags;
    uint32_t mem_lower;
    uint32_t mem_upper;
    uint32_t boot_device;
    uint32_t cmdline;
    uint32_t mods_count;
    uint32_t mods_addr;
    struct {
        uint32_t num;
        uint32_t size;
        uint32_t addr;
        uint32_t shndx;
    } elf_sec;
    uint32_t mmap_length;
    uint32_t mmap_addr;
    uint32_t drives_length;
    uint32_t drives_addr;
    uint32_t config_table;
    uint32_t boot_loader_name;
    uint32_t apm_table;
    struct {
        uint32_t control_info;
        uint32_t mode_info;
        uint32_t mode;
        uint32_t interface_seg;
        uint32_t interface_off;
        uint32_t interface_len;
    } vbe;
} __attribute__((packed));

typedef struct multiboot_info multiboot_info_t;


#endif /* __ARCH_MULTIBOOT_H__ */