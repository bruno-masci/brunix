#include <stdio.h>

#include <CUnit/Basic.h>

#include <asm/memlayout.h>     // for struct std_multiboot_info, struct multiboot_info, MBOOT_LOADER_MAGIC
#include <asm/mmu.h>


int init_suite1(void) {
  return 0;
}

int clean_suite1(void) {
  return 0;
}

#define printk printf

void _panic(const char *message) {
    printf("ERROOOOOORRRRRR!!!! %s", message);
    exit(1);
}


void kmalloc(void) {

}

//// stub for required external function
//void panic(const char *message) {
//}



//static pte_t *
//walkpgdir(pde_t *pgdir, const void *va, int alloc)
typedef uint32_t pde_t;
typedef uint32_t pte_t;


void test_find_dir_entry(void) {
    pde_t *pgdir = malloc(4096);
    const void *virt_addr = 0x00C00000;         // 4th pgdir entry (ten higher bits)
    const void *pgdir_4th_entry = pgdir + 3;

    // under test
    pde_t *pde = pgdir_entry(pgdir, virt_addr);

    CU_ASSERT(pde == pgdir_4th_entry);
}

void test_find_table_entry(void) {
    pte_t *pgtab = malloc(4096);
    const void *virt_addr = 0x00007000;         // 8th pgtable entry (ten bits after ten higher bits)
    const void *pgtab_9th_entry = pgtab + 7;

    // under test
    pte_t *pte = pgtable_entry(pgtab, virt_addr);

    CU_ASSERT(pte == pgtab_9th_entry);
}

void test_paging(void) {
    pde_t *pgdir = malloc(4096);
    const void *virt_addr = 0x00C00000;    // 4th pgdir entry (ten higher bits)

    // under test
    pde_t *pde = pgdir_entry(pgdir, virt_addr);

//    CU_ASSERT(pde == NULL);

    int pd_idx = PDX(virt_addr);
    CU_ASSERT(pd_idx == 3);

    pde_t *pde_ = &pgdir[pd_idx];
    *pde_ = *pde_ | PTE_P;

    // under test
    pde = pgdir_entry(pgdir, virt_addr);

    CU_ASSERT(pde == pde_);
}

/* The main() function for setting up and running the tests.
 * Returns a CUE_SUCCESS on successful running, another
 * CUnit error code on failure.
 */
int main()
{
   CU_pSuite pSuite = NULL;

   /* initialize the CUnit test registry */
   if (CUE_SUCCESS != CU_initialize_registry())
      return CU_get_error();

   /* add a suite to the registry */
   pSuite = CU_add_suite("Suite_1", init_suite1, clean_suite1);
   if (NULL == pSuite) {
      CU_cleanup_registry();
      return CU_get_error();
   }

   /* add the tests to the suite in a particular order */
   if ((NULL == CU_add_test(pSuite, "test of test_find_dir_entry()", test_find_dir_entry)) ||
        (NULL == CU_add_test(pSuite, "test of test_find_table_entry()", test_find_table_entry)) ||
        (NULL == CU_add_test(pSuite, "test of test_paging()", test_paging))) {
      CU_cleanup_registry();
      return CU_get_error();
   }

   /* Run all tests using the CUnit Basic interface */
   CU_basic_set_mode(CU_BRM_VERBOSE);
   CU_basic_run_tests();
   int total_test_failures = CU_get_number_of_failures();
   CU_cleanup_registry();
   int fwk_result_status = CU_get_error();
   int result_status = fwk_result_status != 0 ? fwk_result_status : total_test_failures;

//    unsigned int CU_get_number_of_suites_failed(void)
//    unsigned int CU_get_number_of_tests_failed(void)
//    unsigned int CU_get_number_of_failures(void)

    return result_status;    // 0 is OK
}
