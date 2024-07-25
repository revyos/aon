/*******************************************************
 *  CSI-ROTS Test Case --  csi_kernel_msgq_flush interface test
 *******************************************************/

#include "msgq_test.h"
#include "dtest.h"
#include "test_util.h"

void msgq_flush_interface_test()
{
    int i;
    k_status_t ret;
    k_msgq_handle_t handle;

    /**************  Test Data preparation  ****************/
    handle = csi_kernel_msgq_new(1,5);
    csi_kernel_msgq_put(handle, "test\0", 0, 0);

    handle_status_check_t check_list[] = {
    /**************** msgq_handle is NULL********************/
    {NULL, 0,0},
    /**************** flush msg queue ********************/
    {handle, 0, 1},
    /**************** msgq is empty  *******************/
    {handle, 0, 1},
    };


    /****************  Test Execution  **********************/
    for(i=0;i<sizeof(check_list)/sizeof(handle_status_check_t);i++)
    {
		next_test_case_notify();
        ret = csi_kernel_msgq_flush(check_list[i].handle);
        if(check_list[i].equal) {
			BUFQUEUE_VAL_CHK(check_list[i].status == ret);
			if(check_list[i].status == ret)
			{
				test_case_success++;
				PRINT_RESULT("msgq_flush_interface", PASS);
			}
			else
			{
				test_case_fail++;
				PRINT_RESULT("msgq_flush_interface", FAIL);
			}
		}
        else {
			BUFQUEUE_VAL_CHK(check_list[i].status != ret);
			if(check_list[i].status != ret)
			{
				test_case_success++;
				PRINT_RESULT("msgq_flush_interface", PASS);
			}
			else
			{
				test_case_fail++;
				PRINT_RESULT("msgq_flush_interface", FAIL);
			}
		}
    }

	next_test_case_wait();
}
