/*******************************************************
 *  CSI-ROTS Test Case --  yield tasks test
 *******************************************************/

#include "task_test.h"
#include <stdio.h>
#include "dtest.h"
#include "test_util.h"

static uint8_t check = 1;
static void test_func()
{
    check = 0;

    csi_kernel_task_yield ();
    csi_kernel_task_del(csi_kernel_task_get_cur());
}

void yield_task_test()
{
    k_task_handle_t     handle;
   
    csi_kernel_task_new(test_func, "test_task", NULL, CSI_TEST_TASK_MIN_PRIO,
            0, NULL, CSI_TEST_TASK_STACK, &handle);
    csi_kernel_task_yield();

	next_test_case_notify();
	MYASSERT(check == 0);
	if(check == 0)
	{
		test_case_success++;
		PRINT_RESULT("yield_task_test", PASS);
	}
	else
	{
		test_case_fail++;
		PRINT_RESULT("yield_task_test", FAIL);
	}

	next_test_case_wait();
}     
   
