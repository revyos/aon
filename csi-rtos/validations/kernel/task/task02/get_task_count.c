/*******************************************************
 *  CSI-ROTS Test Case --   get task account and delete test
 *******************************************************/

#include "task_test.h"
#include <stdio.h>
#include "dtest.h"
#include "test_util.h"


static void test_func(void)
{
    csi_kernel_task_suspend(csi_kernel_task_get_cur());
}

void get_task_count_test()
{
    uint32_t num,f_num;
    k_task_handle_t  handle, handle1;

    //Idle + User_Task + Self
    f_num = csi_kernel_task_get_count();

    csi_kernel_task_new((k_task_entry_t)test_func, "test_task", NULL,
		    KPRIO_NORMAL, 0,NULL, CSI_TEST_TASK_STACK, &handle);

    //After New a task
	next_test_case_notify();
    num = csi_kernel_task_get_count();

	MYASSERT(f_num+1 == num);
	if(f_num+1 == num)
	{
		test_case_success++;
		PRINT_RESULT("get_task_count", PASS);
	}
	else
	{
		test_case_fail++;
		PRINT_RESULT("get_task_count", FAIL);
	}

    //Task is not running
    csi_kernel_task_new((k_task_entry_t)test_func, "test_task1", NULL,
		    KPRIO_LOW0, 0,NULL, CSI_TEST_TASK_STACK, &handle1);

	next_test_case_notify();
    num = csi_kernel_task_get_count();

	MYASSERT(f_num+2 == num);
	if(f_num+2 == num)
	{
		test_case_success++;
		PRINT_RESULT("get_task_count", PASS);
	}
	else
	{
		test_case_fail++;
		PRINT_RESULT("get_task_count", FAIL);
	}

    //Delete handle
    csi_kernel_task_del(handle);

	next_test_case_notify();
    num = csi_kernel_task_get_count();
	MYASSERT(f_num+1 == num);
	if(f_num+1 == num)
	{
		test_case_success++;
		PRINT_RESULT("get_task_count", PASS);
	}
	else
	{
		test_case_fail++;
		PRINT_RESULT("get_task_count", FAIL);
	}

    //Delete handle1
    csi_kernel_task_del(handle1);
    num = csi_kernel_task_get_count();
	next_test_case_notify();
	MYASSERT(f_num == num);
	if(f_num == num)
	{
		test_case_success++;
		PRINT_RESULT("get_task_count", PASS);
	}
	else
	{
		test_case_fail++;
		PRINT_RESULT("get_task_count", FAIL);
	}

	next_test_case_wait();

}
