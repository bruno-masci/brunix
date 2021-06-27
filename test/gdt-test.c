#include <stdio.h>

#include <CUnit/Basic.h>

#include <arch/x86/gdt.h>     // for struct std_multiboot_info, struct multiboot_info, MBOOT_LOADER_MAGIC
#include <arch/x86/memlayout.h>     // for struct std_multiboot_info, struct multiboot_info, MBOOT_LOADER_MAGIC


int init_suite1(void) {
  return 0;
}

int clean_suite1(void) {
  return 0;
}

// stub for required external function
void panic(const char *message) {
}

struct gdt_ptr_struct get_gdt_ptr(uint16_t total_gdt_entries, uint32_t gdt_base_addr);

void test_get_gdt_ptr(void) {
    uint16_t total_gdt_entries = 5;
    size_t gdt_desc_size = sizeof(struct gdt_desc_struct);

    // under test
    struct gdt_ptr_struct gdtPtr = get_gdt_ptr(total_gdt_entries, (uint32_t)1);

    CU_ASSERT(gdtPtr.limit == ((gdt_desc_size * total_gdt_entries) - 1));
    CU_ASSERT(gdtPtr.base == VIRT_TO_PHYS_WO(1));
}

void fill_gdt_desc(struct gdt_desc_struct *descriptor, uint32_t base, uint32_t limit_20, uint32_t access_8, uint32_t flags_4);

void test_fill_gdt_desc(void) {
    struct gdt_desc_struct desc;

    // Let's take binary numbers 101010... (4, 8, 20 and 32 bits) as the input
    uint32_t base_linear_addr = 0xAAAAAAAA;
    uint32_t limit_20 = 0xAAAAA;
    uint32_t access_8 = 0xAA;
    uint32_t flags_4 = 0xA;

    // under test
    fill_gdt_desc(&desc, base_linear_addr, limit_20, access_8, flags_4);

    CU_ASSERT(desc.access_8 == 0xAA);
    CU_ASSERT(desc.base_low_16 == 0xAAAA);
    CU_ASSERT(desc.base_middle_8 == 0xAA);
    CU_ASSERT(desc.base_high_8 == 0xAA);
    CU_ASSERT(desc.limit_low_16 == 0xAAAA);
    CU_ASSERT(desc.limit_high_4 == 0xA);
    CU_ASSERT(desc.flags_4 == 0xA);
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
   if ((NULL == CU_add_test(pSuite, "test of get_gdt_ptr()", test_get_gdt_ptr)) ||
        (NULL == CU_add_test(pSuite, "test of fill_gdt_desc()", test_fill_gdt_desc))) {
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
