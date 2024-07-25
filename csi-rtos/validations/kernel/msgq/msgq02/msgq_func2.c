/*******************************************************
 *  CSI-ROTS Test Case --  msgq basic function test
 *  A task put 1 message, and B C task get message
 *******************************************************/

#include "msgq_test.h"
#include <string.h>
#include <stdlib.h>
#include "dtest.h"
#include "test_util.h"


static k_msgq_handle_t handle;
static k_task_handle_t task1, task2, task3;

static void msgq_put()
{
	next_test_case_notify();
    k_status_t ret;
    ret = csi_kernel_msgq_put(handle,"test\0",0,0);
	
	BUFQUEUE_VAL_CHK(ret == 0);
	if(ret == 0)
	{
		test_case_success++;
		PRINT_RESULT("msgq_func_test2", PASS);
	}
	else
	{
		test_case_fail++;
		PRINT_RESULT("msgq_func_test2", FAIL);
	}

    csi_kernel_task_suspend(task1);

    csi_kernel_delay_ms(20);

    csi_kernel_task_del(task3);
    // printf("Test finished\n");
    //asm("bkpt");

    csi_kernel_task_del(task1);
}

static void msgq_get1()
{
    k_status_t ret;

    char *out;

    csi_kernel_delay(1);

	next_test_case_notify();
    out = (char *)malloc(5);
    ret = csi_kernel_msgq_get(handle, out, -1);
	BUFQUEUE_VAL_CHK(ret == 0);
	if(ret == 0)
	{
		test_case_success++;
		PRINT_RESULT("msgq_func_test2", PASS);
	}
	else
	{
		test_case_fail++;
		PRINT_RESULT("msgq_func_test2", FAIL);
	}

    csi_kernel_task_resume(task1);

    free(out);
    csi_kernel_task_del(task2);
}

static void msgq_get2()
{
    k_status_t ret;

    char *out;

    csi_kernel_delay(2);

    out = (char *)malloc(5);
    ret = csi_kernel_msgq_get(handle, out, -1);
    /**********  This shall never entered **********/

	next_test_case_notify();
	BUFQUEUE_VAL_CHK(ret != 0);
	if(ret != 0)
	{
		test_case_success++;
		PRINT_RESULT("msgq_func_test2", PASS);
	}
	else
	{
		test_case_fail++;
		PRINT_RESULT("msgq_func_test2", FAIL);
	}
    csi_kernel_task_resume(task1);
    free(out);
    csi_kernel_task_del(task2);
}


static char put_stack[CSI_TEST_MSGQ_STACK];
static char get_stack1[CSI_TEST_MSGQ_STACK];
static char get_stack2[CSI_TEST_MSGQ_STACK];

void msgq_func_test2()
{
    k_status_t ret;
    handle = csi_kernel_msgq_new(1,5);
    
    ret = csi_kernel_task_new((void *)&msgq_put, "msgq_put", NULL, CSI_TEST_MSGQ_PRIO,0,
                put_stack,CSI_TEST_MSGQ_STACK,&task1);
    ret = csi_kernel_task_new((void *)&msgq_get1, "msgq_get1", NULL, CSI_TEST_MSGQ_PRIO,0,
             get_stack1,CSI_TEST_MSGQ_STACK, &task2);
    ret = csi_kernel_task_new((void *)&msgq_get2, "msgq_get2", NULL, CSI_TEST_MSGQ_PRIO,0,
                 get_stack2,CSI_TEST_MSGQ_STACK,&task3);

	next_test_case_wait();
}


