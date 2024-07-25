/*******************************************************************
 *  CSI-ROTS Test Case --  terminate sub task by main task test
 *******************************************************************/

/* procedures:
1. create new sub tasks
2. suspend new created sub task
3. terminate the suspended sub task by main task

This case is to verify it support to terminate tasks other than itself*/

#include "task_test.h"
#include <stdio.h>
#include "dtest.h"
#include "test_util.h"

static void test_func()
{
    csi_kernel_task_terminate(csi_kernel_task_get_cur());

	MYASSERT(1 == 0);
	if(1 != 0)
	{
		test_case_fail++;
		PRINT_RESULT("terminate_task_test", FAIL);
	}
}

void terminate_task_test()
{
	next_test_case_notify();

    k_task_handle_t    handle;

	int flag_terminate = test_case_fail;

    csi_kernel_task_new(test_func, "task_test", NULL,KPRIO_NORMAL,
                0, NULL, CSI_TEST_TASK_STACK, &handle);

	MYASSERT(flag_terminate == test_case_fail);
	if(flag_terminate == test_case_fail)
	{
		test_case_success++;
		PRINT_RESULT("terminate_task_test", PASS);
	}

	MYASSERT(csi_kernel_task_get_stat(handle) == KTASK_ST_TERMINATED);
	if(csi_kernel_task_get_stat(handle) == KTASK_ST_TERMINATED)
	{
		test_case_success++;
		PRINT_RESULT("terminate_task_test", PASS);
	}
	else
	{
		test_case_fail++;
		PRINT_RESULT("terminate_task_test", FAIL);
	}
    csi_kernel_task_del(handle);

	next_test_case_wait();
}     
   
