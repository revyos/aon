/*******************************************************
 *  CSI-ROTS Test Case --  get_current_task_handle test
 *******************************************************/

#include "task_test.h"
#include <stdio.h>
#include "dtest.h"
#include "test_util.h"

k_task_handle_t  handle, handle1;

static void test_func() {
	next_test_case_notify();
    handle = csi_kernel_task_get_cur();
	MYASSERT(handle != NULL);
	if(handle != NULL)
	{
		test_case_success++;
		PRINT_RESULT("get_current_handle_test", PASS);
	}
	else
	{
		test_case_fail++;
		PRINT_RESULT("get_current_handle_test", FAIL);
	}

	MYASSERT(handle1 == handle);
	if(handle1 == handle)
	{
		test_case_success++;
		PRINT_RESULT("get_current_handle_test", PASS);
	}
	else
	{
		test_case_fail++;
		PRINT_RESULT("get_current_handle_test", FAIL);
	}

    csi_kernel_task_del(handle);
}

void get_current_handle_test()
{ 
    handle = handle1 = NULL;
    csi_kernel_task_new((k_task_entry_t)test_func, "test_task", NULL,
                      KPRIO_NORMAL,0, NULL,CSI_TEST_TASK_STACK,&handle1);

	next_test_case_wait();
}

