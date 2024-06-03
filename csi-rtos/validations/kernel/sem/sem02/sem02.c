#include <test_util.h>
#include "sem_test.h"
#include "dtest.h"

//k_sem_handle_t g_sem_handle;

void sem_test(void)
{
	dtest_suite_info_t test_suite_info = {
		"sem_test", NULL, NULL, NULL, NULL
	};

	dtest_suite_t *test_suite = dtest_add_suite(&test_suite_info);

	dtest_case_info_t test_case_info_arry[]= {
		{"sem_new_interface_test", sem_new_interface_test, 1},
		{"sem_del_interface_test", sem_del_interface_test, 1},
		{"sem_post_interface_test", sem_post_interface_test, 1},
		{"sem_wait_interface_test", sem_wait_interface_test, 1},
		{"sem_func_test", sem_func_test, 1},
		{"sem_func_test2", sem_func_test, 1},
		{NULL, NULL, 0}
	};

	dtest_add_cases(test_suite, test_case_info_arry);
}

int app_main(void)
{
	dtest_register_cb(sem_test);
	dtest();
	
	return 0;
}
