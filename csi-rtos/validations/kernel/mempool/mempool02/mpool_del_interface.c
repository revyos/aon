/*******************************************************
 *  CSI-ROTS Test Case --  csi_kernel_mpool_del interface test
 *******************************************************/

#include "mpool_test.h"
#include "dtest.h"
#include "test_util.h"

static char mpool[256];

void mpool_del_interface_test()
{
    int i;
    k_status_t ret;
    k_mpool_handle_t handle;
    handle = csi_kernel_mpool_new(mpool, 16,16);
    handle_status_check_t check_list[] = {
    /**************** mpool_handle is NULL********************/
    {NULL, 0,0},
    /**************** mpool_handle is available ********************/
    {handle, 0, 1},
    };


    for(i=0;i<sizeof(check_list)/sizeof(handle_status_check_t);i++)
    {
		next_test_case_notify();
        ret = csi_kernel_mpool_del(check_list[i].handle);
        if(check_list[i].equal) {
			MYASSERT(check_list[i].status == ret);
			if(check_list[i].status == ret)
			{
				test_case_success++;
				PRINT_RESULT("mpool_del_interface", PASS);
			}
			else
			{
				test_case_fail++;
				PRINT_RESULT("mpool_del_interface", FAIL);
			}
		}
        else {
			MYASSERT(check_list[i].status != ret);
			if(check_list[i].status != ret)
			{
				test_case_success++;
				PRINT_RESULT("mpool_del_interface", PASS);
			}
			else
			{
				test_case_fail++;
				PRINT_RESULT("mpool_del_interface", PASS);
			}
		}
    }

	next_test_case_wait();
}
