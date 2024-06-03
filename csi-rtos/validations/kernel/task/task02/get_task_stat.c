/*************************************************************
 *  CSI-ROTS Test Case --  get_task_stat_of_ready test
 *************************************************************/

//this case is to verify the task status of ready.
//suspend: KTASK_ST_INACTIVE 0
//RUNNING: KTASK_ST_RUNNING  2
//ready:   KTASK_ST_READY    1
//terminated: KTASK_ST_TERMINATED 4

#include "task_test.h"
#include <stdio.h>
#include "dtest.h"
#include "test_util.h"

#define TEST_STACK_SIZE 512

static void test_func()
{ 
    csi_kernel_task_suspend(csi_kernel_task_get_cur());

	next_test_case_notify();
	MYASSERT(KTASK_ST_RUNNING == csi_kernel_task_get_stat(csi_kernel_task_get_cur()));
	if(KTASK_ST_RUNNING == csi_kernel_task_get_stat(csi_kernel_task_get_cur()))
	{
		test_case_success++;
		PRINT_RESULT("get_task_stat", PASS);
	}
	else
	{
		test_case_fail++;
		PRINT_RESULT("get_task_stat", FAIL);
	}
    csi_kernel_task_del(csi_kernel_task_get_cur());
}

static void test_func2()
{
    csi_kernel_task_exit();
}

void get_task_stat_test()
{
    k_task_stat_t ret;
    //Task handle is NULL
	next_test_case_notify();
    ret = csi_kernel_task_get_stat(NULL);
	MYASSERT(KTASK_ST_ERROR == ret);
	if(KTASK_ST_ERROR == ret)
	{
		test_case_success++;
		PRINT_RESULT("get_task_stat", PASS);
	}
	else
	{
		test_case_fail++;
		PRINT_RESULT("get_task_stat", FAIL);
	}
    
    //Task is running
	next_test_case_notify();
    ret = csi_kernel_task_get_stat(csi_kernel_task_get_cur());
	MYASSERT(KTASK_ST_RUNNING == ret);
	if(KTASK_ST_RUNNING == ret)
	{
		test_case_success++;
		PRINT_RESULT("get_task_stat", PASS);
	}
	else
	{
		test_case_fail++;
		PRINT_RESULT("get_task_stat", FAIL);
	}

    k_task_handle_t handle;
    //Task is suspended
    csi_kernel_task_new(test_func, "task_test",NULL, KPRIO_HIGH7,
                        0, NULL,CSI_TEST_TASK_STACK,&handle);
	next_test_case_notify();
    ret = csi_kernel_task_get_stat(handle);
    MYASSERT(KTASK_ST_BLOCKED == ret);
	if(KTASK_ST_BLOCKED == ret)
	{
		test_case_success++;
		PRINT_RESULT("get_task_stat", PASS);
	}
	else
	{
		test_case_fail++;
		PRINT_RESULT("get_task_stat", FAIL);
	}

    csi_kernel_task_resume(handle);
    
    //Task is deleted

	next_test_case_notify();
    ret = csi_kernel_task_get_stat(handle);
	MYASSERT(KTASK_ST_TERMINATED == ret);
	if(KTASK_ST_TERMINATED == ret)
	{
		test_case_success++;
		PRINT_RESULT("get_task_stat", PASS);
	}
	else
	{
		test_case_fail++;
		PRINT_RESULT("get_task_stat", FAIL);
	}

    //Task is only in ready task queue
    csi_kernel_task_new(test_func, "task_test",NULL, KPRIO_LOW0,
                        0, NULL,CSI_TEST_TASK_STACK,&handle);

	next_test_case_notify();
    ret = csi_kernel_task_get_stat(handle);
	MYASSERT(KTASK_ST_READY == ret);
	if(KTASK_ST_READY == ret)
	{
		test_case_success++;
		PRINT_RESULT("get_task_stat", PASS);
	}
	else
	{
		test_case_fail++;
		PRINT_RESULT("get_task_stat", FAIL);
	}

    //Task is terminated
    csi_kernel_task_terminate(handle);
	next_test_case_notify();
	ret = csi_kernel_task_get_stat(handle);
	MYASSERT(KTASK_ST_TERMINATED == ret);
	if(KTASK_ST_TERMINATED == ret)
	{
		test_case_success++;
		PRINT_RESULT("get_task_stat", PASS);
	}
	else
	{
		test_case_fail++;
		PRINT_RESULT("get_task_stat", FAIL);
	}

    //Task is exited
    csi_kernel_task_new(test_func2, "task_test",NULL, KPRIO_HIGH7,
                        0, NULL,CSI_TEST_TASK_STACK,&handle);
	next_test_case_notify();
    ret = csi_kernel_task_get_stat(handle);
    MYASSERT(KTASK_ST_TERMINATED == ret);
	if(KTASK_ST_TERMINATED == ret)
	{
		test_case_success++;
		PRINT_RESULT("get_task_stat", PASS);
	}
	else
	{
		test_case_fail++;
		PRINT_RESULT("get_task_stat", FAIL);
	}

	next_test_case_wait();
}
