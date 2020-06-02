/**
 * @author Bruno Masci
 * @brief Entry point to the C kernel
 *
 * This file contains the kernel's main program.
 */


#include <stdint.h>                 // for uint32_t

#include <arch/x86/multiboot.h>     // for multiboot_info_t, struct required_multiboot_info, MBOOT_LOADER_MAGIC

#include <brunix/defs.h>            // for CHECK_FLAG()
#include <brunix/string.h>          // for strcpy()
#include <brunix/console.h>         // for debug(), cprintf()
#include <brunix/kernel.h>          // for panic()


uint32_t total_ram_memory;


void save_multiboot_info(multiboot_info_t *mboot_info_ptr, struct required_multiboot_info *brunix_multiboot_info_ptr) {
    total_ram_memory = mboot_info_ptr->mem_upper;
    cprintf("MBOOT RAM memory: %d\n", total_ram_memory);
//    return;

    debug("\nmboot mem %u, flags %b\n", mboot_info_ptr->mem_upper, mboot_info_ptr->flags);
//    debug("\nmboot str %s, mem %u, flags %b\n", mboot_info_ptr->cmdline, mboot_info_ptr->mem_upper, mboot_info_ptr->flags);

    /* is the command-line defined? */
//#define MULTIBOOT_INFO_CMDLINE                  0x00000004

//    mboot_info_ptr = mboot_info_ptr + 0x80000000;//FIXME
    if (CHECK_FLAG(mboot_info_ptr->flags, 2)) { //FIXME entra con o sin command line args!
        strcpy(brunix_multiboot_info_ptr->cmdline, (const char *) mboot_info_ptr->cmdline);
        debug("CMDLINE: %s\n", brunix_multiboot_info_ptr->cmdline);
    } else {
        debug_noargs("NO CMDLINE\n");
    }

    brunix_multiboot_info_ptr->mem_upper = mboot_info_ptr->mem_upper;
    brunix_multiboot_info_ptr->flags = mboot_info_ptr->flags;


    cprintf("\n\n\n\n\n\n\n\n\n\n\nmboot str %s, mem %u, flags %b\n", brunix_multiboot_info_ptr->cmdline, brunix_multiboot_info_ptr->mem_upper, brunix_multiboot_info_ptr->flags);
}

void verify_loader(uint32_t magic) {
    if (magic != MBOOT_LOADER_MAGIC) {
        panic("Invalid magic number! A Multiboot-compliant loader is needed...");
    }
}