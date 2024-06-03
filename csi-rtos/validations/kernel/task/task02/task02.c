#include "task_test.h"
#include <test_util.h>
#include "dtest.h"

void task_test(void)
{
	dtest_suite_info_t test_suite_info = {
		"test_task", NULL, NULL, NULL, NULL
	};

	dtest_suite_t *test_suite = dtest_add_suite(&test_suite_info);

	dtest_case_info_t test_case_info_array[] = {
		{"task_new_interface_test", task_new_interface_test ,1},
		{"task_del_interface_test", task_del_interface_test, 1},
		{"task_get_stat_interface_test",task_get_stat_interface_test, 1},
		{"task_set_prio_interface_test",task_set_prio_interface_test, 1},
		{"task_get_prio_interface_test", task_get_prio_interface_test, 1},
		{"task_suspend_interface_test",task_suspend_interface_test, 1},
		{"task_terminate_interface_test",task_terminate_interface_test, 1},
		{"task_resume_interface_test", task_resume_interface_test, 1},
		{"task_list_interface_test", task_list_interface_test, 1},
		{"create_task_test", create_task_test, 1},
		{"delete_task_test", delete_task_test, 1},
		{"exit_task_test", exit_task_test, 1},
		{"get_task_count_test", get_task_count_test, 1},
		{"get_current_handle_test", get_current_handle_test, 1},
		{"get_task_stat_test", get_task_stat_test, 1},
	#if 0
		{"list_task_test", list_task_test, 1},
	#endif
		{"resume_task_test", resume_task_test, 1},
		{"set_get_task_prio_test", set_get_task_prio_test, 1},
		{"suspend_task_test", suspend_task_test, 1},
	#if 0
		{"task_quanta_test", task_quanta_test, 1},
	#endif
		{"terminate_task_test", terminate_task_test, 1},
		{"yield_task_test", yield_task_test, 1},
		{NULL, NULL, 0}
	};

	dtest_add_cases(test_suite, test_case_info_array);
}

int app_main(void)
{
	dtest_register_cb(task_test);
	dtest();

	return 0;
}
