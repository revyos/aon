/*******************************************************
 *  CSI-ROTS Test Case --  csi_kernel_sem_post interface test
 *******************************************************/

#include "sem_test.h"
#include "dtest.h"
#include "test_util.h"


void sem_post_interface_test()
{
    int i;
    k_status_t ret;
    k_sem_handle_t handle1;
    handle1 = csi_kernel_sem_new(1, 0);
    handle_status_check_t check_list[] = {
    /**************** sem_handle is NULL********************/
    {NULL, 0,0},
    /**************** sem handle is available ***************/
    {handle1, 0, 1},
    /**************** sem count exceeded max count ************/
    //{handle1, 0, 0},
    };

    for(i=0;i<sizeof(check_list)/sizeof(handle_status_check_t);i++)
    {	
		next_test_case_notify();
		ret = csi_kernel_sem_post(check_list[i].handle);
		if(check_list[i].equal) {
			MYASSERT(check_list[i].status == ret);
			if(check_list[i].status == ret)
			{
				test_case_success++;
				PRINT_RESULT("sem_post_interface",PASS);
			}
			else
			{
				test_case_fail++;
				PRINT_RESULT("sem_post_interface",FAIL);
			}
		}
		else {
			MYASSERT(check_list[i].status  != ret);
			if(check_list[i].status != ret)
			{
				test_case_success++;
				PRINT_RESULT("sem_post_interface",PASS);
			}
			else
			{
				test_case_fail++;
				PRINT_RESULT("sem_post_interface",FAIL);
			}
		}
    }

	next_test_case_wait();
}
