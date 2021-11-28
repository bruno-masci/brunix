/**
 * @author Bruno Masci
 * @brief Multiboot Standard related functions
 * @see main.c
 * @see multiboot_entry_point.S
 */


#include <stdint.h>                 // for uint32_t
#include <stdbool.h>

#include <asm/multiboot.h>     // for struct std_multiboot_info, struct multiboot_info, MBOOT_LOADER_MAGIC, MAX_CMDLINE_LEN

#include <string.h>          // for strcpy()
#include <brunix/kernel.h>          // for panic()


/**
 * @brief Copy the multiboot info set by the bootloader to a location we manage.
 *
 * The bootloader fills a data structure (struct std_multiboot_info) with contextual information about the system.
 * We can't trust its location (in this case, GRUB chooses that location) and maybe at some time we unintentionally overwrite
 * it. It is better to move that to a location we manage!
 */
void save_multiboot_info(struct std_multiboot_info *std_mboot_info, struct multiboot_info *mboot_info) {
    strncpy(mboot_info->cmdline, (const char *) std_mboot_info->cmdline, MAX_CMDLINE_LEN);
    mboot_info->mem_upper = std_mboot_info->mem_upper;
    mboot_info->flags = std_mboot_info->flags;
}

/**
 * @brief Verify this kernel was loaded by a Multiboot-compliant bootloader.
 */
void verify_loader(uint32_t magic) {
    if (magic != MBOOT_LOADER_MAGIC) {
        panic("Invalid magic number! A Multiboot-compliant loader is needed...");
    }
}
