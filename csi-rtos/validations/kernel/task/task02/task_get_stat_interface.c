/*******************************************************
 *  CSI-ROTS Test Case --  csi_kernel_task_get_stat interface test
 *******************************************************/

#include "task_test.h"
#include "dtest.h"
#include "test_util.h"

void task_get_stat_interface_test()
{
    int i;
    k_status_t ret;

    handle_status_check_t check_list[] = {
    /**************** handle is NULL ****************/
 //   {NULL,NULL,0},
     {NULL,0,0},
    };

    for(i=0;i<sizeof(check_list)/sizeof(handle_status_check_t);i++)
    {
		next_test_case_notify();
        ret = csi_kernel_task_get_stat(check_list[i].handle);
        if (check_list[i].equal) {
			MYASSERT(check_list[i].status == ret);
			if(check_list[i].status == ret)
			{
				test_case_success++;
				PRINT_RESULT("task_get_stat_interface", PASS);
			}
			else
			{
				test_case_fail++;
				PRINT_RESULT("task_get_stat_interface", FAIL);
			}
        } else {
			MYASSERT(check_list[i].status != ret);
			if(check_list[i].status != ret)
			{
				test_case_success++;
		        PRINT_RESULT("task_get_stat_interface", PASS);
			}
		    else
			{
			    test_case_fail++;
				PRINT_RESULT("task_get_stat_interface", FAIL);
			}
        }
    }

	next_test_case_wait();
}
