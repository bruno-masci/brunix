/**
 * @author Bruno Masci
 * @brief Entry point to the C kernel
 *
 * This file contains the kernel's main program.
 */


#include <stdint.h>                 // for uint32_t

#include <arch/x86/multiboot.h>     // for multiboot_info_t, struct required_multiboot_info, MBOOT_LOADER_MAGIC

#include <brunix/string.h>          // for strcpy()
#include <brunix/kernel.h>          // for panic()


void save_multiboot_info(multiboot_info_t *mboot_info_ptr, struct required_multiboot_info *brunix_multiboot_info_ptr) {
    strcpy(brunix_multiboot_info_ptr->cmdline, (const char *) mboot_info_ptr->cmdline);
    brunix_multiboot_info_ptr->mem_upper = mboot_info_ptr->mem_upper;
    brunix_multiboot_info_ptr->flags = mboot_info_ptr->flags;
}

void verify_loader(uint32_t magic) {
    if (magic != MBOOT_LOADER_MAGIC) {
        panic("Invalid magic number! A Multiboot-compliant loader is needed...");
    }
}