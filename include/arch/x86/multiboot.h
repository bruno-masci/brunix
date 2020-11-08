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

#define MAX_CMDLINE_LEN 256


#ifndef __ASSEMBLER__

    #define MBOOT_LOADER_MAGIC  0x2BADB002

    #include <stdint.h>     // for uint32_t

    struct std_multiboot_info {
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

    struct multiboot_info {
        uint32_t mem_upper;
        uint32_t flags;
        char cmdline[MAX_CMDLINE_LEN];
    };

    void save_multiboot_info(struct std_multiboot_info *std_mboot_info, struct multiboot_info *mboot_info);

    void verify_loader(uint32_t magic);

#endif /* !__ASSEMBLER__ */


#endif /* __ARCH_MULTIBOOT_H__ */
