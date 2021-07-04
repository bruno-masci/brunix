/**
 * @brief Definitions and functions related to paging
 * @author Bruno Masci
 *
 * This file defines paging related stuff.
 */

#ifndef __ARCH_PAGING_H__
#define __ARCH_PAGING_H__

#include <stdint.h>     // for uint32_t


typedef uint32_t pde_t;

#define PGDIR_PRESENT_FLAG 			0x1
#define PGDIR_WRITABLE_FLAG 		0x2
#define PGDIR_USER_FLAG 			0x4	//(1 << 2) variante


#define PAGE_SIZE					0x1000 // 4 KB
#define	RAM_MAXPAGE					0x100000
#define PAGE(addr)					(uint32_t)(((uint32_t)addr) >> 12)


struct page_dir_struct {
    unsigned int present_flag : 1;
    unsigned int read_write_flag : 1;
    unsigned int user_supervisor_flag : 1;
    unsigned int unused_flags : 3;
    unsigned int zero_flag : 1;
    unsigned int page_size_flag : 1;
    unsigned int more_unused_flags : 4;
    unsigned int page_table_base_address : 20;
} __attribute__((packed));
//typedef struct page_dir_struct page_dir_t;

struct page_table_struct {
    unsigned int present_flag : 1;
    unsigned int read_write_flag : 1;
    unsigned int user_supervisor_flag : 1;
    unsigned int unused_flags : 9;
    unsigned int physical_page_address : 20;
} __attribute__((packed));
//typedef struct page_table_struct page_table_t;


enum PageSize {NORMAL, HUGE};

/**
   Sets up the environment, page directories etc and
   enables paging.
**/
void paging_init(uint32_t mem_upper_in_bytes);

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


void enable_paging(void);

void load_page_directory(struct page_dir_struct *page_dir);


#ifndef __ASSEMBLER__
typedef uint32_t pte_t;

static inline bool is_page_aligned(const char *v) {
    return ((uint32_t) v % PAGE_SIZE) == 0;
}
#endif

#endif /* __ARCH_PAGING_H__ */
