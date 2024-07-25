/*******************************************************
 *  CSI-ROTS Test Case --   exit current tasks test
 *******************************************************/

#include "task_test.h"
#include <stdio.h>
#include "dtest.h"
#include "test_util.h"

void test_func()
{
	next_test_case_notify();
    csi_kernel_task_exit();
	MYASSERT(0 == 1);
	test_case_fail++;
	PRINT_RESULT("exit_task", FAIL);
}     
   
void exit_task_test()
{
    k_task_handle_t  handle;
    csi_kernel_task_new(test_func, "exit_task", NULL, KPRIO_HIGH0,
                       0, NULL, CSI_TEST_TASK_STACK,&handle);

	next_test_case_wait();
	test_case_success++;
	PRINT_RESULT("exit_task", PASS);
}
