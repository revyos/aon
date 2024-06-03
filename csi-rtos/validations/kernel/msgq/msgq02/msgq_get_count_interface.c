/*******************************************************
 *  CSI-ROTS Test Case --  csi_kernel_msgq_get_count interface test
 *******************************************************/

#include "msgq_test.h"
#include "dtest.h"
#include "test_util.h"


void msgq_get_count_interface_test()
{
    int i;
    int ret;
    k_msgq_handle_t handle0;

    handle0 = csi_kernel_msgq_new(1,5);

    handle_status_check_t check_list[] = {
    /**************** msgq_handle is NULL ********************/
    {NULL, 0,0},
    /*************** msgq is empty ***********************/
    {handle0, 0, 1},
    };

    for(i=0;i<sizeof(check_list)/sizeof(handle_status_check_t);i++)
    {
		next_test_case_notify();
        ret = csi_kernel_msgq_get_count(check_list[i].handle);
        if(i<1) {
			BUFQUEUE_VAL_CHK(ret != 0);
			if(ret != 0)
			{
				test_case_success++;
				PRINT_RESULT("msgq_get_count_interface", PASS);
			}
			else
			{
				test_case_fail++;
				PRINT_RESULT("msgq_get_count_interface", FAIL);
			}
		}
        else {
			BUFQUEUE_VAL_CHK(ret == 0);
			if(ret == 0)
			{
				test_case_success++;
				PRINT_RESULT("msgq_get_count_interface", PASS);
			}
			else
			{
				test_case_fail++;
				PRINT_RESULT("msgq_get_count_interface", FAIL);
			}
		}
    }

	next_test_case_wait();
}
