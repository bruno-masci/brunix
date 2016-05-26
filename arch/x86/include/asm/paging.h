/**
 * @brief Definitions and functions related to paging
 * @author Bruno Masci
 *
 * This file defines paging related stuff.
 */

#ifndef __ARCH_PAGING_H__
#define __ARCH_PAGING_H__

#include <asm/stddef.h>
#include <asm/isr.h>	// for registers_t




#define PGDIR_PRESENT_FLAG 		0x1
#define PGDIR_WRITABLE_FLAG 	0x2
#define PGDIR_USER_FLAG 		0x4	//(1 << 2) variante


#define PAGE_SIZE						0x1000 // 4 KB
#define TABLE_SHIFT_R(address)			(((uint32_t)address) >> 12)



struct page_dir_struct {
	unsigned int present_flag : 1;
	unsigned int read_write_flag : 1;
	unsigned int user_supervisor_flag : 1;
	unsigned int unused_flags : 9;
	unsigned int page_table_base_address : 20;
} __attribute__((packed));
typedef struct page_dir_struct page_dir_t;

struct page_table_struct {
	unsigned int present_flag : 1;
	unsigned int read_write_flag : 1;
	unsigned int user_supervisor_flag : 1;
	unsigned int unused_flags : 9;
	unsigned int physical_page_address : 20;
} __attribute__((packed));
typedef struct page_table_struct page_table_t;



/**
   Sets up the environment, page directories etc and
   enables paging.
**/
void paging_init(void);

/**
   Causes the specified page directory to be loaded into the
   CR3 register.
**/
//void switch_page_directory(page_directory_t *new);

/**
   Retrieves a pointer to the page required.
   If make == 1, if the page-table in which this page should
   reside isn't created, create it!
**/
//page_t *get_page(uint32_t address, int make, page_dir_t *dir);

/**
   Handler for page faults.
**/
void page_fault(struct registers_t regs);


#endif /* __ARCH_PAGING_H__ */
