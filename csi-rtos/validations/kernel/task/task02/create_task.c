/*******************************************************
 *  CSI-ROTS Test Case --  create_multi_task test
 *******************************************************/

/* Purpose: This case is to verify the scenario that multiple tasks are crated, and deleted then.
* Steps:
1. Create task0, and set its priority as the same as the main task which crate it(create_multi_task_test). it will not enter func to run task0, but start to create task1 instead.
2. Create task1-task4, and enter runc to run.
3. switch to run the task0 or main task (create_multi_task_test), and it depends on the timequanta. */ 

#include "task_test.h"
#include <stdio.h>
#include "dtest.h"
#include "test_util.h"


#define TEST_STACK_SIZE 512

static char new_if_stask[TEST_STACK_SIZE];
static char new_if_stask1[TEST_STACK_SIZE];
static char new_if_stask2[TEST_STACK_SIZE];
static char new_if_stask3[TEST_STACK_SIZE];
static char new_if_stask4[TEST_STACK_SIZE];

static void func( int* arg)
{
    csi_kernel_task_del(csi_kernel_task_get_cur());
}


void create_task_test()
{
    /***  Priority of task_1 - task_4 is higher than this main task while task_0 is the same as main task ****/
    task_new_check_t check_list[] = {
    {(void *)func,"task_0",NULL,KPRIO_LOW5,0,new_if_stask, TEST_STACK_SIZE},
    {(void *)func,"task_1",NULL,KPRIO_NORMAL_BELOW0,0,new_if_stask1,TEST_STACK_SIZE},
    {(void *)func,"task_2",NULL,KPRIO_NORMAL7,0,new_if_stask2,TEST_STACK_SIZE},
    {(void *)func,"task_3",NULL,KPRIO_NORMAL_ABOVE7,0,new_if_stask3,TEST_STACK_SIZE},
    {(void *)func,"task_4",NULL,KPRIO_HIGH7,0,new_if_stask4,TEST_STACK_SIZE},
    };
   
    int i;
   
    k_task_handle_t    handle[5];
    
    k_status_t status_new[5];

    for(i=0;i<sizeof(check_list)/sizeof(task_new_check_t);i++)
    {
		next_test_case_notify();
	/*******************    Create new sub task    ********************/
	status_new[i]= csi_kernel_task_new((void *)check_list[i].task,check_list[i].name,&i,check_list[i].prio,check_list[i].time_quanta,check_list[i].stack,check_list[i].stack_size,&handle[i]);
		MYASSERT(status_new[i] == 0);
		if(status_new[i] == 0)
		{
			test_case_success++;
			PRINT_RESULT("create_task", PASS);
		}
		else
		{
			test_case_fail++;
			PRINT_RESULT("create_task", FAIL);
		}
    }

	next_test_case_wait();

}
   
