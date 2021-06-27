#include <stdio.h>
#include <stdbool.h>

#include <CUnit/Basic.h>

#include <arch/x86/multiboot.h>     // for struct std_multiboot_info, struct multiboot_info, MBOOT_LOADER_MAGIC


int init_suite1(void) {
  return 0;
}

int clean_suite1(void) {
  return 0;
}

static int panic_function_called_count;

void reset_test_context(void) {
    panic_function_called_count = 0;
}

// stub for required external function
void panic(const char *message) {
    panic_function_called_count++;
}

void test_verify_valid_loader(void) {
    reset_test_context();

    verify_loader(MBOOT_LOADER_MAGIC);

    CU_ASSERT(panic_function_called_count == 0);
}

void test_verify_invalid_loader(void) {
    reset_test_context();

    verify_loader(12345);

    CU_ASSERT(panic_function_called_count == 1);
}

void test_save_multiboot_info(void) {
    struct std_multiboot_info std_mboot_info;
    struct multiboot_info mboot_info;

    char *cmdline = "arg1 arg2";
    uint32_t mem_upper = 1024;
    uint32_t flags = 12345;

    std_mboot_info.cmdline = (uint32_t) cmdline;    // pointer to the string
    std_mboot_info.mem_upper = mem_upper;
    std_mboot_info.flags = flags;

    // under test
    save_multiboot_info(&std_mboot_info, &mboot_info);

    CU_ASSERT(strcmp(mboot_info.cmdline, cmdline) == 0);
    CU_ASSERT(mboot_info.mem_upper == mem_upper);
    CU_ASSERT(mboot_info.flags == flags);
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
   if ((NULL == CU_add_test(pSuite, "Verifying valid loader should be okay", test_verify_valid_loader)) ||
       (NULL == CU_add_test(pSuite, "Verifying invalid loader should fail", test_verify_invalid_loader)) ||
       (NULL == CU_add_test(pSuite, "Saving Multiboot Info", test_save_multiboot_info))) {
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
