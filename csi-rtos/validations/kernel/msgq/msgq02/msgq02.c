#include "msgq_test.h"
#include <test_util.h>
#include "dtest.h"


void buf_queue_test(void)
{
	dtest_suite_info_t test_suite_info = {
        "buf_queue_test", NULL, NULL, NULL, NULL
	};

	dtest_suite_t *test_suite = dtest_add_suite(&test_suite_info);

	dtest_case_info_t test_case_info_array[] = {
		{"msgq_new_interface_test",msgq_new_interface_test,1},
		{"msgq_del_interface_test",msgq_del_interface_test,1},
		{"msgq_put_interface_test",msgq_put_interface_test,1},
		{"msgq_get_interface_test",msgq_get_interface_test,1},
		{"msgq_get_count_interface_test",msgq_get_count_interface_test,1},
		{"msgq_flush_interface_test",msgq_flush_interface_test,1},
		{"msgq_func_test",msgq_func_test,1},
		{"msgq_func_test2",msgq_func_test2,1},
		{"msgq_func_test3",msgq_func_test3,1},
		{"msgq_func_test4",msgq_func_test4,1},
		{NULL,NULL,0}
   };
   
   dtest_add_cases(test_suite, test_case_info_array);
}

int app_main(void)
{
	dtest_register_cb(buf_queue_test);
	dtest();

	return 0;
}















