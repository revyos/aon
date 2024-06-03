/*******************************************************
 *  CSI-ROTS Test Case --  set current task prio test
 *******************************************************/

#include "task_test.h"
#include <stdio.h>
#include "dtest.h"
#include "test_util.h"

static uint8_t check=1;
static void test_func()
{
	next_test_case_notify();
	MYASSERT(csi_kernel_task_get_prio(csi_kernel_task_get_cur()) == KPRIO_HIGH7);
	if(csi_kernel_task_get_prio(csi_kernel_task_get_cur()) == KPRIO_HIGH7)
	{
		test_case_success++;
		PRINT_RESULT("set_get_task_prio_test", PASS);
	}
	else
	{
		test_case_fail++;
		PRINT_RESULT("set_get_task_prio_test", FAIL);
	}
    check =0;
    csi_kernel_task_del(csi_kernel_task_get_cur());
}

static void test_func1()
{
	next_test_case_notify();
	MYASSERT(csi_kernel_task_get_prio(csi_kernel_task_get_cur()) == KPRIO_HIGH0);
	if(csi_kernel_task_get_prio(csi_kernel_task_get_cur()) == KPRIO_HIGH0)
	{
		test_case_success++;
		PRINT_RESULT("set_get_task_prio_test", PASS);
	}
	else
	{
		test_case_fail++;
		PRINT_RESULT("set_get_task_prio_test", FAIL);
	}
    csi_kernel_task_set_prio(csi_kernel_task_get_cur(), KPRIO_LOW0);
}

void set_get_task_prio_test()
{        
    k_task_handle_t handle;

    csi_kernel_task_new(test_func,"test_task",NULL,
             KPRIO_LOW0, 0, NULL, CSI_TEST_TASK_STACK, &handle );

    csi_kernel_task_set_prio(handle, KPRIO_HIGH7);

    while(check);

    csi_kernel_task_new(test_func1,"task1",NULL,
             KPRIO_HIGH0, 0 ,NULL, CSI_TEST_TASK_STACK, &handle);

	next_test_case_notify();
	MYASSERT(csi_kernel_task_get_prio(csi_kernel_task_get_cur()) == CSI_TEST_TASK_MIN_PRIO);
	if(csi_kernel_task_get_prio(csi_kernel_task_get_cur()) == CSI_TEST_TASK_MIN_PRIO)
	{
		test_case_success++;
		PRINT_RESULT("set_get_task_prio_test", PASS);
	}
	else
	{
		test_case_fail++;
		PRINT_RESULT("set_get_task_prio_test", FAIL);
	}

	next_test_case_wait();
}
