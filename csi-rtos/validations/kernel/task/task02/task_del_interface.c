/*******************************************************
 *  CSI-ROTS Test Case --  csi_kernel_task_del interface test
 *******************************************************/

#include "task_test.h"
#include "dtest.h"
#include "test_util.h"

void task_del_interface_test()
{
    k_status_t ret;
    int i;
    handle_status_check_t check_list[] = {
    /**************** task_handle is NULL********************/
    {NULL,0, 0},
    /**************** task_handle is not available********************/
    //{handle,0, 0},
    };

    for(i=0;i<sizeof(check_list)/sizeof(handle_status_check_t);i++)
    {
		next_test_case_notify();
        ret = csi_kernel_task_del(check_list[i].handle);
	if (check_list[i].equal) {
		MYASSERT(check_list[i].status == ret);
		if(check_list[i].status == ret)
		{
			test_case_success++;
			PRINT_RESULT("task_del_interface", PASS);
		}
		else
		{
			test_case_fail++;
			PRINT_RESULT("task_del_interface", FAIL);
		}
	} else {
		MYASSERT(check_list[i].status != ret);
		if(check_list[i].status != ret)
		{
			test_case_success++;
			PRINT_RESULT("task_del_interface", PASS);
		}
		else
		{
			test_case_fail++;
			PRINT_RESULT("task_del_interface", FAIL);
		}

      }
    }

	next_test_case_wait();
}
