/*******************************************************
 *  CSI-ROTS Test Case --  csi_kernel_msgq_del interface test
 *******************************************************/

#include "msgq_test.h"
#include "dtest.h"
#include "test_util.h"



void msgq_del_interface_test()
{
    int i;
    k_status_t ret;
    k_msgq_handle_t handle1;
    handle1 = csi_kernel_msgq_new(1,5);

    handle_status_check_t check_list[] = {
    /**************** msgq_handle is NULL********************/
    {NULL, 0,0},
    /**************** msgq_handle is available ********************/
    {handle1, 0, 1},
    /**************** msgq_handle is already deleted  **************/
    //{handle1, 0, 0},
    };

    for(i=0;i<sizeof(check_list)/sizeof(handle_status_check_t);i++)
    {
		next_test_case_notify();
        ret = csi_kernel_msgq_del(check_list[i].handle);
        if(check_list[i].equal) {
			BUFQUEUE_VAL_CHK(check_list[i].status == ret);
			if(check_list[i].status == ret)
			{
				test_case_success++;
				PRINT_RESULT("msgq_del_interface", PASS);
			}
			else
			{
				test_case_fail++;
				PRINT_RESULT("msgq_del_interface", FAIL);
			}
		}
        else {
			BUFQUEUE_VAL_CHK(check_list[i].status != ret);
			if(check_list[i].status != ret)
			{
				test_case_success++;
				PRINT_RESULT("msgq_del_interface", PASS);
			}
			else
			{
				test_case_fail++;
				PRINT_RESULT("msgq_del_interface", FAIL);
			}
		}
    }

	next_test_case_wait();
}
