/*******************************************************
 *  CSI-ROTS Test Case --  k_task_list interface test
 *******************************************************/

#include "task_test.h"
#include "dtest.h"
#include "test_util.h"

void task_list_interface_test()
{
	k_task_handle_t task_handle_array[10];
	uint32_t i, task_num;
	typedef struct task_list {
		k_task_handle_t     *array;
		uint32_t            items;
	}task_list_t;

	task_list_t check_list[] = {
		{NULL,0},
		{task_handle_array,0},
		{task_handle_array,10}
	};

    for(i=0;i<sizeof(check_list)/sizeof(task_list_t);i++) {
		next_test_case_notify();
        task_num = csi_kernel_task_list(check_list[i].array,check_list[i].items);
		MYASSERT(check_list[i].items >= task_num);
		if(check_list[i].items == task_num)
		{
			test_case_success++;
			PRINT_RESULT("task_list_interface", PASS);
		}
		else
		{
			test_case_fail++;
			PRINT_RESULT("task_list_interface", FAIL);
		}
    }

	next_test_case_wait();
}

