#include "mpool_test.h"
#include <test_util.h>
#include "dtest.h"

void mm_blk_test(void)
{
	dtest_suite_info_t test_suite_info = {
		"mm_blk_test", NULL, NULL, NULL, NULL
	};

	dtest_suite_t *test_suite = dtest_add_suite(&test_suite_info);

	dtest_case_info_t test_case_info_array[] = {
		{"mpool_new_interface_test", mpool_new_interface_test, 1},
		{"mpool_del_interface_test", mpool_del_interface_test, 1},
		{"mpool_alloc_interface_test", mpool_alloc_interface_test, 1},
		{"mpool_free_interface_test", mpool_free_interface_test, 1},
		{"mpool_func_test", mpool_func_test,1},
		{NULL, NULL, 0}
	};

	dtest_add_cases(test_suite, test_case_info_array);
}

int app_main(void)
{
	dtest_register_cb(mm_blk_test);
	dtest();

	return 0;
}

