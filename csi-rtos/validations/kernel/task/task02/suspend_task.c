/*******************************************************
 *  CSI-ROTS Test Case --  suspend multi tasks test
 *******************************************************/

#include "task_test.h"
#include <stdio.h>
#include "dtest.h"
#include "test_util.h"


static void pend_func()
{
	next_test_case_notify();
    csi_kernel_task_suspend(csi_kernel_task_get_cur());
	MYASSERT(1 == 0);
	if(1 != 0)
	{
		test_case_fail++;
		PRINT_RESULT("suspend_task_test", FAIL);
	}
    //ASSERT(1==0);
}

static void pend_func2()
{
	next_test_case_notify();
    csi_kernel_delay(1);
	MYASSERT(1 == 0);
	if(1 != 0)
	{
		test_case_fail++;
		PRINT_RESULT("suspend_task_test", FAIL);
	}
   // ASSERT(1 ==0);
}

void suspend_task_test()
{
    k_task_handle_t handle;

	int flag_suspend = test_case_fail;

    //self suspend
    csi_kernel_task_new(pend_func, "self_suspend", NULL, KPRIO_HIGH7,
                  0,NULL,CSI_TEST_TASK_STACK,&handle);
    
    csi_kernel_delay(1);

    csi_kernel_task_del(handle);
	MYASSERT(1 == 1);
	if(flag_suspend == test_case_fail)
	{
		test_case_success++;
		PRINT_RESULT("suspend_task_test", PASS);
	}

	flag_suspend = test_case_fail;

    //suspend sub-task
    csi_kernel_task_new(pend_func2, "self_suspend", NULL, KPRIO_HIGH7,
                  0,NULL,CSI_TEST_TASK_STACK,&handle);
    csi_kernel_task_suspend(handle);

    csi_kernel_delay(2);
	MYASSERT(1 == 1);
	if(flag_suspend == test_case_fail)
	{
		test_case_success++;
		PRINT_RESULT("suspend_task_test", PASS);
	}

	next_test_case_wait();
}
