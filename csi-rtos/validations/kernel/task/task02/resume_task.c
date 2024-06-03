/*******************************************************
 *  CSI-ROTS Test Case --  resume multi tasks test
 *******************************************************/

#include "task_test.h"
#include <stdio.h>
#include "dtest.h"
#include "test_util.h"

static volatile uint8_t check = 0;

static void test_func ()
{
    k_task_handle_t handle;
    handle = csi_kernel_task_get_cur();
    csi_kernel_task_suspend(handle);
    check =1;
    csi_kernel_task_del(handle);
}

void resume_task_test()
{
	next_test_case_notify();

    k_task_handle_t handle;
    
    csi_kernel_task_new((k_task_entry_t)test_func, "task_test", NULL, 
            KPRIO_NORMAL, 0, NULL, CSI_TEST_TASK_STACK,&handle);
    
    csi_kernel_delay(1);
    csi_kernel_task_resume(handle);

	MYASSERT(check == 1);
	if(check == 1)
	{
		test_case_success++;
		PRINT_RESULT("resume_task_test", PASS);
	}
	else
	{
		test_case_fail++;
		PRINT_RESULT("resume_task_test", FAIL);
	}

	next_test_case_wait();
}
  

