/*******************************************************
 *  CSI-ROTS Test Case --  csi_kernel_msgq_new interface test
 *******************************************************/

#include "msgq_test.h"
#include "dtest.h"
#include "test_util.h"


void msgq_new_interface_test()
{
    int i;
    k_msgq_handle_t handle;
    int32_t check_list[][3] = {
    /**************** msg count is 0 ********************/
    {0, 10,0},
    /**************** msg size is 0 ********************/
    {10, 0,0},
    /**************** msg count larger than size ********************/
    //{5, 4, 0},
    /**************** msg count and size are 0x1  **************/
    {1,1,1},
    /**************** msg count and size are 0xff  **************/
#if ( CONFIG_KERNEL_RHINO || CONFIG_KERNEL_UCOS )
    {0xff, 0xff, 1},
#endif
    /**************** msg count is negative number  **************/
    {-1, 10, 0},
    /*************** msg size is negative number *********/
    {10,-1,0},
    /**************** Normal setting **********************/
    {10,40, 1},
    };

    for(i=0;i<sizeof(check_list)/(sizeof(uint32_t)*3);i++)
    {
		next_test_case_notify();
		//printf("==%d\n",i);
        handle = csi_kernel_msgq_new(check_list[i][0],check_list[i][1]);
        if(check_list[i][2]) {
			BUFQUEUE_VAL_CHK(handle != NULL);
			if(handle != NULL)
			{
				test_case_success++;
				PRINT_RESULT("msgq_new_interface", PASS);
			}
			else
			{
				test_case_fail++;
				PRINT_RESULT("msgq_new_interface", FAIL);
			}
		}
        else {
			BUFQUEUE_VAL_CHK(handle == NULL);
			if(handle == NULL)
			{
				test_case_success++;
				PRINT_RESULT("msgq_new_interface", PASS);
			}
			else
			{
				test_case_fail++;
				PRINT_RESULT("msgq_new_interface", FAIL);
			}
		}
    }

	next_test_case_wait();
}
