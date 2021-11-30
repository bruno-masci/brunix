#include <stdio.h>

#include <CUnit/Basic.h>

#include "../include/asm/gdt.h"     // for struct std_multiboot_info, struct multiboot_info, MBOOT_LOADER_MAGIC
#include "../include/asm/memlayout.h"

int init_suite1(void) {
  return 0;
}

int clean_suite1(void) {
  return 0;
}

extern struct gdt_ptr_struct get_gdt_ptr(uint16_t total_gdt_entries, uint32_t gdt_base_addr);

void test_get_gdt_ptr(void) {
    uint16_t total_gdt_entries = 5;
    size_t gdt_desc_size = sizeof(struct gdt_desc_struct);

    // under test
    struct gdt_ptr_struct gdtPtr;
    uint32_t base = 0xFFFFFFFF;
    fill_gdt_ptr(&gdtPtr, base, total_gdt_entries);

    CU_ASSERT(gdtPtr.limit == ((gdt_desc_size * total_gdt_entries) - 1));
    CU_ASSERT(gdtPtr.base == VIRT_TO_PHYS_WO(base));
}

extern void gdt_fill_table(struct gdt_desc_struct *gdt_table, struct tss *initial_tss);

void test_gdt_fill_table(void) {
    struct gdt_desc_struct gdt_table[6];

    struct tss default_tss = {0, };

    // under test
    gdt_fill_table(gdt_table, &default_tss);

    CU_ASSERT(gdt_table[0].flags == 0);
    CU_ASSERT(gdt_table[0].access == 0);
    CU_ASSERT(gdt_table[0].base_low_16 == 0);
    CU_ASSERT(gdt_table[0].base_middle_8 == 0);
    CU_ASSERT(gdt_table[0].base_high_8 == 0);
    CU_ASSERT(gdt_table[0].limit_low_16 == 0);
    CU_ASSERT(gdt_table[0].limit_high_4 == 0);

    CU_ASSERT(gdt_table[1].flags == 0xC);
    CU_ASSERT(gdt_table[1].access == 0x9A);
    CU_ASSERT(gdt_table[1].base_low_16 == 0);
    CU_ASSERT(gdt_table[1].base_middle_8 == 0);
    CU_ASSERT(gdt_table[1].base_high_8 == 0);
    CU_ASSERT(gdt_table[1].limit_low_16 == 0xFFFF);
    CU_ASSERT(gdt_table[1].limit_high_4 == 0xF);

    CU_ASSERT(gdt_table[2].flags == 0xC);
    CU_ASSERT(gdt_table[2].access == 0x92);
    CU_ASSERT(gdt_table[2].base_low_16 == 0);
    CU_ASSERT(gdt_table[2].base_middle_8 == 0);
    CU_ASSERT(gdt_table[2].base_high_8 == 0);
    CU_ASSERT(gdt_table[2].limit_low_16 == 0xFFFF);
    CU_ASSERT(gdt_table[2].limit_high_4 == 0xF);

    CU_ASSERT(gdt_table[3].flags == 0xC);
    CU_ASSERT(gdt_table[3].access == 0xFA);
    CU_ASSERT(gdt_table[3].base_low_16 == 0);
    CU_ASSERT(gdt_table[3].base_middle_8 == 0);
    CU_ASSERT(gdt_table[3].base_high_8 == 0);
    CU_ASSERT(gdt_table[3].limit_low_16 == 0xFFFF);
    CU_ASSERT(gdt_table[3].limit_high_4 == 0XF);

    CU_ASSERT(gdt_table[4].flags == 0xC);
    CU_ASSERT(gdt_table[4].access == 0xF2);
    CU_ASSERT(gdt_table[4].base_low_16 == 0);
    CU_ASSERT(gdt_table[4].base_middle_8 == 0);
    CU_ASSERT(gdt_table[4].base_high_8 == 0);
    CU_ASSERT(gdt_table[4].limit_low_16 == 0xFFFF);
    CU_ASSERT(gdt_table[4].limit_high_4 == 0XF);

    CU_ASSERT(gdt_table[5].flags == 0x4);
    CU_ASSERT(gdt_table[5].access == 0x89);
    CU_ASSERT(gdt_table[5].base_low_16 == (((uint32_t) &default_tss) & 0xFFFF));
    CU_ASSERT(gdt_table[5].base_middle_8 == ((((uint32_t) &default_tss) >> 16) & 0xFF));
    CU_ASSERT(gdt_table[5].base_high_8 == ((((uint32_t) &default_tss) >> 24) & 0xFF));
    CU_ASSERT(gdt_table[5].limit_low_16 == (sizeof(struct tss) - 1));
    CU_ASSERT(gdt_table[5].limit_high_4 == 0);
}

extern void gdt_set_descr(struct gdt_desc_struct *descriptor, uint32_t base, uint32_t limit_20, uint32_t access_8, uint32_t flags_4);

void test_gdt_set_descr(void) {
    struct gdt_desc_struct desc;

    // Let's take binary numbers 1010, 10101010, ... (4, 8, 20 and 32 bits) as the input
    uint32_t base_linear_addr = 0xAAAAAAAA;
    uint32_t limit_20 = 0xAAAAA;
    uint32_t access_8 = 0xAA;
    uint32_t flags_4 = 0xA0;

    // under test
    gdt_set_descr(&desc, base_linear_addr, limit_20, access_8, flags_4);

    CU_ASSERT(desc.access == 0xAA);
    CU_ASSERT(desc.base_low_16 == 0xAAAA);
    CU_ASSERT(desc.base_middle_8 == 0xAA);
    CU_ASSERT(desc.base_high_8 == 0xAA);
    CU_ASSERT(desc.limit_low_16 == 0xAAAA);
    CU_ASSERT(desc.limit_high_4 == 0xA);
    CU_ASSERT(desc.flags == 0xA);
}

/* The main() function for setting up and running the tests.
 * Returns a CUE_SUCCESS on successful running, another
 * CUnit error code on failure.
 */
int main(void) {
   CU_pSuite pSuite = NULL;

   /* initialize the CUnit test registry */
   if (CUE_SUCCESS != CU_initialize_registry()) {
       return CU_get_error();
   }

   /* add a suite to the registry */
   pSuite = CU_add_suite("Suite_1", init_suite1, clean_suite1);
   if (NULL == pSuite) {
      CU_cleanup_registry();
      return CU_get_error();
   }

   /* add the tests to the suite in a particular order */
   if ((NULL == CU_add_test(pSuite, "test of gdt_fill_table()", test_gdt_fill_table)) ||
    (NULL == CU_add_test(pSuite, "test of get_gdt_ptr()", test_get_gdt_ptr)) ||
    (NULL == CU_add_test(pSuite, "test of gdt_set_descr()", test_gdt_set_descr))) {
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
