/*******************************************************
 *  CSI-ROTS Test Case --  csi_kernel_task_new interface test
 *******************************************************/

#include "task_test.h"
#include "dtest.h"
#include "test_util.h"

k_task_handle_t handle[8];

void task_new_null(void) 
{
    printf("enter task_new_null function\n");
    csi_kernel_task_del(csi_kernel_task_get_cur());
}

void task_new_arg(void *arg)
{
    printf("enter task_new_arg function\n");
    int *int_check;
    int_check = arg;
    if(*int_check != 1)
    {
        printf("csi_kernel_task_new-2 failed\n");
        //fail_count++;
		test_case_fail++;
    }
    csi_kernel_task_del(csi_kernel_task_get_cur());
}
#define TASK_NEW_STACK_SIZE  512

static char new_if_stack[TASK_NEW_STACK_SIZE];

void task_new_interface_test()
{
    int check_arg = 1;
    int i;
	k_status_t ret;

    
    printf("\ncsi_kernel_task_new interface test get started!\n");    
    task_null_check_t check_list[] = {
    /************** Task function is NULL  **************/
    {NULL,"task_new_0",NULL,CSI_TEST_TASK_MAX_PRIO,0,new_if_stack,TASK_NEW_STACK_SIZE, 0},
    /************** Task Name is NULL *******************/
    {(k_task_entry_t)&task_new_null,NULL,NULL,CSI_TEST_TASK_MAX_PRIO,0,new_if_stack,TASK_NEW_STACK_SIZE, 1},
    /************** Task Arg is not Null ****************/
    {&task_new_arg,"task_new_2",(void *) &check_arg,CSI_TEST_TASK_MAX_PRIO,0,new_if_stack,TASK_NEW_STACK_SIZE,1},
    /************ Time_quanta is Null ***********/
    {(k_task_entry_t)&task_new_null,"task_new_6",NULL,CSI_TEST_TASK_MAX_PRIO,0,new_if_stack,TASK_NEW_STACK_SIZE,1},//0},//NULL is equal to 0
    /************ Time_quanta is max value of unsigned int ***********/
    {(k_task_entry_t)&task_new_null,"task_new_7",NULL,CSI_TEST_TASK_MAX_PRIO,0xffffffff,new_if_stack,TASK_NEW_STACK_SIZE,1},
    /************ Stack is NULL ***********/
    {(k_task_entry_t)&task_new_null,"task_new_8",NULL,CSI_TEST_TASK_MAX_PRIO,0,NULL,TASK_NEW_STACK_SIZE,1},
    /************ Stack_size is 0 ***********/
#ifndef CONFIG_KERNEL_FREERTOS
    {(k_task_entry_t)&task_new_null,"task_new_10",NULL,CSI_TEST_TASK_MAX_PRIO,0,new_if_stack,0,0},
#endif
    };

    for(i=0;i<sizeof(check_list)/sizeof(task_null_check_t);i++)
    {
		next_test_case_notify();
        ret = csi_kernel_task_new((void *)check_list[i].task,check_list[i].name, check_list[i].arg,
                                check_list[i].prio,check_list[i].time_quanta,
                                check_list[i].stack, check_list[i].stack_size,&handle[i]);
        if(check_list[i].nullcheck) {
			MYASSERT(ret == 0);
			if(ret == 0)
			{
				test_case_success++;
				PRINT_RESULT("task_new_interface", PASS);
			}
			else
			{
				test_case_fail++;
				PRINT_RESULT("task_new_interface", FAIL);
			}

			next_test_case_notify();
			MYASSERT(handle[i] != NULL);
			if(handle[i] != NULL)
			{
				test_case_success++;
				PRINT_RESULT("task_new_interface", PASS);
			}
			else
			{
				test_case_fail++;
				PRINT_RESULT("task_new_interface", FAIL);
			}
		}else{
			MYASSERT(ret != 0);
			if(ret != 0)
			{
				test_case_success++;
				PRINT_RESULT("task_new_interface", PASS);
			}
			else
			{
				test_case_fail++;
				PRINT_RESULT("task_new_interface", FAIL);
			}
        }
    }
	
	next_test_case_wait();
}
