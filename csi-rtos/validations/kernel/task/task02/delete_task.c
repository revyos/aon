/*******************************************************
 *  CSI-ROTS Test Case --  delete_current_task test
 *******************************************************/

#include "task_test.h"
#include <stdio.h>
#include "dtest.h"
#include "test_util.h"

k_task_handle_t  handle;

static void func()
{
    csi_kernel_task_suspend(handle);
}

static void func1()
{
	MYASSERT(1 == 0);
	test_case_fail++;
	PRINT_RESULT("delete_task", FAIL);
}

static void func2()
{
    csi_kernel_task_exit();
}

static char delete_task[CSI_TEST_TASK_STACK];
void delete_task_test()
{
    k_status_t ret;
    //delete suspend task
    ret = csi_kernel_task_new(func, "delete_task", NULL, KPRIO_HIGH0,
                       0, delete_task, CSI_TEST_TASK_STACK,&handle);

    ret = csi_kernel_task_del(handle);
	
	next_test_case_notify();
	MYASSERT(KTASK_ST_TERMINATED == csi_kernel_task_get_stat(handle));
	if(KTASK_ST_TERMINATED == csi_kernel_task_get_stat(handle))
	{
		test_case_success++;
		PRINT_RESULT("delete_task", PASS);
	}
	else
	{
		test_case_success++;
		PRINT_RESULT("delete_task", FAIL);
	}

    //delete ready task
    ret = csi_kernel_task_new(func1, "delete_task1", NULL, KPRIO_LOW0,
                       0, NULL, CSI_TEST_TASK_STACK,&handle);
    ret = csi_kernel_task_del(handle);

	next_test_case_notify();
	MYASSERT(KTASK_ST_TERMINATED == csi_kernel_task_get_stat(handle));
	if(KTASK_ST_TERMINATED == csi_kernel_task_get_stat(handle))
	{
		test_case_success++;
		PRINT_RESULT("delete_task", PASS);
	}
	else
	{
		test_case_success++;
		PRINT_RESULT("delete_task", FAIL);
	}

    //delete exit task
    ret = csi_kernel_task_new(func2, "delete_task2", NULL, KPRIO_HIGH7,
                       0, NULL, CSI_TEST_TASK_STACK,&handle);
    ret = csi_kernel_task_del(handle);
	next_test_case_notify();
#ifndef CONFIG_KERNEL_RHINO
	MYASSERT(ret == 0);
	if(ret == 0)
	{
		test_case_success++;
		PRINT_RESULT("delete_task", PASS);
	}
	else
	{
		test_case_success++;
		PRINT_RESULT("delete_task", FAIL);
	}
#else
	MYASSERT(ret == -1);
	if(ret == -1)
	{
		test_case_success++;
		PRINT_RESULT("delete_task", PASS);
	}
	else
	{
		test_case_success++;
		PRINT_RESULT("delete_task", FAIL);
	}
#endif

	next_test_case_notify();
	MYASSERT(KTASK_ST_TERMINATED == csi_kernel_task_get_stat(handle));
	if(KTASK_ST_TERMINATED == csi_kernel_task_get_stat(handle))
	{
		test_case_success++;
		PRINT_RESULT("delete_task", PASS);
	}
	else
	{
		test_case_success++;
		PRINT_RESULT("delete_task", FAIL);
	}

	next_test_case_wait();
}
