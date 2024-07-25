/*******************************************************
 *  CSI-ROTS Test Case --  csi_kernel_mpool_alloc interface test
 *******************************************************/

#include "mpool_test.h"
#include "dtest.h"
#include "test_util.h"

static char mpool[256];

void mpool_alloc_interface_test()
{
    int i;
    struct mpool_alloc_check {
        k_mpool_handle_t handle;
        int    timeout;
        int    null;
    };

    k_mpool_handle_t handle1;
    handle1 = csi_kernel_mpool_new(mpool, 3,64);

    typedef struct mpool_alloc_check mpool_alloc_check_t;
    mpool_alloc_check_t check_list[] = {
    /**************** mpool_handle is NULL ********************/
    {NULL,0 , 0},
    /*************** Normal settings ***************************/
    {handle1,0,1},
    {handle1,-1,1},
    {handle1,5,1},
    /************** mpool is empty *******************************/
    {handle1, 0,0 },
    };

    void *ret;
    for(i=0;i<sizeof(check_list)/sizeof(mpool_alloc_check_t);i++)
    {
		next_test_case_notify();
        ret = csi_kernel_mpool_alloc(check_list[i].handle, check_list[i].timeout);
        if(check_list[i].null) {
			MYASSERT(ret != NULL);
			if(ret != NULL)
			{
				test_case_success++;
				PRINT_RESULT("mpool_alloc_interface", PASS);
			}
			else
			{
				test_case_fail++;
				PRINT_RESULT("mpool_alloc_interface", FAIL);
			}
		}
        else {
			MYASSERT(ret == NULL);
			if(ret == NULL)
			{
				test_case_success++;
				PRINT_RESULT("mpool_alloc_interface", PASS);
			}
			else
			{
				test_case_fail++;
				PRINT_RESULT("mpool_alloc_interface", FAIL);
			}
		}
    }

	next_test_case_wait();
}
