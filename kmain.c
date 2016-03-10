#include "descriptor_tables.h"
#include "arch/x86/kernel/vga.h"


/* This should be in %eax. */
#define MULTIBOOT_BOOTLOADER_MAGIC              0x36D76289


struct multiboot
{
   uint32_t flags;
   uint32_t mem_lower;
   uint32_t mem_upper;
   uint32_t boot_device;
   uint32_t cmdline;
   uint32_t mods_count;
   uint32_t mods_addr;
   uint32_t num;
   uint32_t size;
   uint32_t addr;
   uint32_t shndx;
   uint32_t mmap_length;
   uint32_t mmap_addr;
   uint32_t drives_length;
   uint32_t drives_addr;
   uint32_t config_table;
   uint32_t boot_loader_name;
   uint32_t apm_table;
   uint32_t vbe_control_info;
   uint32_t vbe_mode_info;
   uint32_t vbe_mode;
   uint32_t vbe_interface_seg;
   uint32_t vbe_interface_off;
   uint32_t vbe_interface_len;
}  __attribute__((packed));

typedef struct multiboot_header multiboot_header_t;


extern const void kernel_start;
extern const void kernel_end;
extern const void bss_start;
extern const void bss_end;
extern char __BUILD_DATE;
extern char __BUILD_TIME;


int kmain(uint32_t magic, struct multiboot *mboot_ptr) {
	vga_init();

	printk("Starting Brunix...\n\n");

	if (magic != MULTIBOOT_BOOTLOADER_MAGIC) {
		printk("Invalid magic number! Multiboot compatible loader is needed...");
	        return 1;
         }

	init_descriptor_tables();

 // TODO initialize .bss section (de eduOS)
        //memset((void*)&bss_start, 0x00, ((size_t) &bss_end - (size_t) &bss_start));


	printk("Kernel starts at %x and ends at %x. Total: %d KB\n", &kernel_start, &kernel_end, -(&kernel_end - &kernel_start) / 1024);
	printk("BSS section starts at %x and ends at %x. Total: %d KB\n", &bss_start, &bss_end, (&bss_end - &bss_start) / 1024);
   printk("Build date: %d, time: %d\n", &__BUILD_DATE, &__BUILD_TIME);


	asm volatile ("int $0x10");
//	asm volatile ("int $0x8");		FALLA

	return 0;
}
