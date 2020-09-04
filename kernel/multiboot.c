/**
 * @author Bruno Masci
 * @brief Multiboot related functions
 *
 * This file contains the mapping from GRUB's Multiboot info.
 */


#include <stdint.h>                 // for uint32_t

#include <arch/x86/multiboot.h>     // for struct std_multiboot_info, struct multiboot_info, MBOOT_LOADER_MAGIC, MAX_CMDLINE_LEN

#include <brunix/string.h>          // for strcpy()
#include <brunix/kernel.h>          // for panic()


void save_multiboot_info(struct std_multiboot_info *std_mboot_info_ptr, struct multiboot_info *mboot_info_ptr) {
    strncpy(mboot_info_ptr->cmdline, (const char *) std_mboot_info_ptr->cmdline, MAX_CMDLINE_LEN);
    mboot_info_ptr->mem_upper = std_mboot_info_ptr->mem_upper;
    mboot_info_ptr->flags = std_mboot_info_ptr->flags;
}

void verify_loader(uint32_t magic) {
    if (magic != MBOOT_LOADER_MAGIC) {
        panic("Invalid magic number! A Multiboot-compliant loader is needed...");
    }
}