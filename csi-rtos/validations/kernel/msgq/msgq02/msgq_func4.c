/*******************************************************
 *  CSI-ROTS Test Case --  msgq basic function test
 *  A task put 1 message, and B C task get message
 *******************************************************/

#include "msgq_test.h"
#include <string.h>
#include "dtest.h"
#include "test_util.h"

static k_msgq_handle_t handle;
static k_task_handle_t task1, task2;

static void msgq_put()
{
	next_test_case_notify();
    csi_kernel_task_suspend(task1);
    csi_kernel_task_resume(task2);
    k_status_t ret;

    int i;
	uint8_t value = 1;

    for(i=0; i<1000; i++)
    {
        ret = csi_kernel_msgq_put(handle,"test\0",0,1000);
		if (ret != 0)
			value = 0;
    }

	BUFQUEUE_VAL_CHK(value == 1);
	if(value == 1)
	{
		test_case_success++;
		PRINT_RESULT("msgq_func_test4", PASS);
	}
	else
	{
		test_case_fail++;
		PRINT_RESULT("msgq_func_test4", FAIL);
	}

    //printf("Msgq func4 test finished\n");

    csi_kernel_task_del(task1);
}

static void msgq_get()
{

    csi_kernel_task_suspend(task2);
    k_status_t ret;

    char *out;
    out = (char *)malloc(5);
    int i;
	uint8_t v1 = 1,v2 = 1;

    for(i=0;i<1000; i++)
    {
        ret = csi_kernel_msgq_get(handle, out, 1000);
		if (ret != 0)
			v1 = 0;

        if(strncmp(out, "test", 4) !=0 )
			v2 = 0;
    }

	next_test_case_notify();
	BUFQUEUE_VAL_CHK(v1 == 1);
	if(v1 ==1)
	{
		test_case_success++;
		PRINT_RESULT("msgq_func_test4", PASS);
	}
	else
	{
		test_case_fail++;
		PRINT_RESULT("msgq_func_test4", FAIL);
	}
	
	next_test_case_notify();
	BUFQUEUE_VAL_CHK(v2 == 1);
	if(v2 == 1)
	{
		test_case_success++;
		PRINT_RESULT("msgq_func_test4", PASS);
	}
	else
	{
		test_case_fail++;
		PRINT_RESULT("msgq_func_test4", FAIL);
	}

    csi_kernel_task_resume(task1);

    free(out);

    //printf("Test finished\n");
    //asm("bkpt");

    csi_kernel_task_del(task2);
}


static char put_stack[CSI_TEST_MSGQ_STACK];
static char get_stack[CSI_TEST_MSGQ_STACK];

void msgq_func_test4()
{
    k_status_t ret;
    handle = csi_kernel_msgq_new(1,5);
    csi_kernel_sched_suspend() ;   
    ret = csi_kernel_task_new((void *)&msgq_put, "msgq_put", NULL, CSI_TEST_MSGQ_PRIO,
            0,put_stack,CSI_TEST_MSGQ_STACK, &task1);
    ret = csi_kernel_task_new((void *)&msgq_get, "msgq_get", NULL, CSI_TEST_MSGQ_PRIO,
             0,get_stack,CSI_TEST_MSGQ_STACK,&task2);

    csi_kernel_sched_resume(0) ;   

    next_test_case_wait();
   
}


