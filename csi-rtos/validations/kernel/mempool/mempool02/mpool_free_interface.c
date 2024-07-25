/*******************************************************
 *  CSI-ROTS Test Case --  csi_kernel_mpool_free interface test
 *******************************************************/

#include "mpool_test.h"
#include "dtest.h"
#include "test_util.h"

static char mpool[256];

void mpool_free_interface_test()
{
    int i;
    k_status_t ret;
    k_mpool_handle_t handle0;
    struct mpool_free_check {
        handle_status_check_t check;
        void *block;
    };

    typedef struct mpool_free_check mpool_free_check_t;
    handle0 = csi_kernel_mpool_new(mpool, 2,64);
    void *block;
    block = csi_kernel_mpool_alloc(handle0, 0);

    mpool_free_check_t check_list[] = {
    /**************** mpool_handle is NULL ********************/
    {{NULL, 0,0},block},
    /*************** Normal setting ***********************/
    {{handle0, 0, 1}, block},
    /*************** block is already freed ***********************/
#if CONFIG_KERNEL_UCOS
    {{handle0, 0, 0}, block},
#endif
    };


    for(i=0;i<sizeof(check_list)/sizeof(mpool_free_check_t);i++)
    {
		next_test_case_notify();
        ret = csi_kernel_mpool_free(check_list[i].check.handle, check_list[i].block);
        if(check_list[i].check.equal) {
			MYASSERT(check_list[i].check.status == ret);
			if(check_list[i].check.status == ret)
			{
				test_case_success++;
				PRINT_RESULT("mpool_free_interface", PASS);
			}
			else
			{
				test_case_fail++;
				PRINT_RESULT("mpool_free_interface",FAIL);
			}
		}
        else {
			MYASSERT(check_list[i].check.status != ret);
			if(check_list[i].check.status != ret)
			{
				test_case_success++;
				PRINT_RESULT("mpool_free_interface", PASS);
			}
			else
			{
				test_case_fail++;
				PRINT_RESULT("mpool_free_interface",FAIL);
			}
		}
    }

	next_test_case_wait();
}
