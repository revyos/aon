/*******************************************************
 *  CSI-ROTS Test Case --  task list test
 *******************************************************/

#include "task_test.h"
#include <stdio.h>
#include "dtest.h"
#include "test_util.h"


#define TEST_STACK_SIZE 512

static void test_func()
{
    csi_kernel_task_suspend(csi_kernel_task_get_cur());
}

static int check_handle(k_task_handle_t *task_array, k_task_handle_t check, uint32_t num)
{
    int i;
    for(i=0;i<num;i++)
    {
        if(check == task_array[i])
            return 1;
    }

    return 0;
}

void list_task_test()
{
    int i;
    k_task_handle_t    task_array[7];
    uint32_t  ret;

	next_test_case_notify();
    ret = csi_kernel_task_list(task_array, 1);
	MYASSERT(ret == 1);
	if(ret == 1)
	{
		test_case_success++;
		PRINT_RESULT("list_task", PASS);
	}
	else
	{
		test_case_fail++;
		PRINT_RESULT("list_task", FAIL);
	}
   
    k_task_handle_t    handle[5];

    handle[0] = csi_kernel_task_get_cur();

    for(i=1;i<5;i++)
    {
	/*******************    Create new sub task    ********************/
	csi_kernel_task_new(test_func, "test_task", NULL, KPRIO_NORMAL, 0, NULL,TEST_STACK_SIZE,&handle[i]);
    }

    ret = csi_kernel_task_list(task_array, 10);
	next_test_case_notify();
	MYASSERT(ret == 7);
	if(ret == 7)
	{
		test_case_success++;
		PRINT_RESULT("list_task", PASS);
	}
	else
	{
		test_case_fail++;
		PRINT_RESULT("list_task", FAIL);
	}
    for(i=0;i<5;i++) {
		next_test_case_notify();
		MYASSERT(check_handle(task_array, handle[i], 7) == 1);
		if(check_handle(task_array, handle[i], 7) == 1)
		{
			test_case_success++;
			PRINT_RESULT("list_task", PASS);
		}
		else
		{
			test_case_fail++;
			PRINT_RESULT("list_task", FAIL);
		}
    }

	next_test_case_wait();
}
   
