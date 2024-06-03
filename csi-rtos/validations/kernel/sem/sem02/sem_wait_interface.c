/*******************************************************
 *  CSI-ROTS Test Case --  csi_kernel_sem_post interface test
 *******************************************************/

#include "sem_test.h"
#include "dtest.h"
#include "test_util.h"


void sem_wait_interface_test()
{
    int i;
    k_status_t ret;
    k_sem_handle_t handle0;
    struct sem_wait_check {
        handle_status_check_t check;
        uint32_t              ticks;
    };
    handle0 = csi_kernel_sem_new(4,4);
    struct sem_wait_check check_list[] = {
    /**************** sem_handle is NULL********************/
    {{NULL, 0,0},10},
    /*************** sem_handle is available ***********************/
    {{handle0, 0, 1}, 5},
    {{handle0, 0, 1}, 0},
    {{handle0, 0, 1}, -1},
    {{handle0, 0, 1}, 0x7fffffff},
    /**************** sem is 0 ****************************/
    {{handle0, 0, 0}, 5},
    };


    for(i=0;i<sizeof(check_list)/sizeof(struct sem_wait_check);i++)
    {
		next_test_case_notify();
        ret = csi_kernel_sem_wait(check_list[i].check.handle, check_list[i].ticks);
        if(check_list[i].check.equal) {
			MYASSERT(check_list[i].check.status == ret);
			if(check_list[i].check.status == ret)
			{
				test_case_success++;
				PRINT_RESULT("sem_wait_interface",PASS);
			}
			else
			{
				test_case_fail++;
				PRINT_RESULT("sem_wait_interface",FAIL);
			}
		}
        else {
			MYASSERT(check_list[i].check.status != ret);
			if(check_list[i].check.status != ret)
			{
				test_case_success++;
				PRINT_RESULT("sem_wait_interface",PASS);
			}
			else
			{
				test_case_fail++;
				PRINT_RESULT("sem_wait_interface",FAIL);
			}
		}
    }

	next_test_case_wait();
}
