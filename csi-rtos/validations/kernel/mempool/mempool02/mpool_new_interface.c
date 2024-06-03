/*******************************************************
 *  CSI-ROTS Test Case --  csi_kernel_mpool_new interface test
 *******************************************************/

#include "mpool_test.h"
#include "dtest.h"
#include "test_util.h"

static char mpool[256];

void mpool_new_interface_test()
{
    int i;
    k_mpool_handle_t handle;
    struct mpool_test {
    char *mpool;
    uint32_t count;
    uint32_t size;
    int      equal;
    };

    typedef struct mpool_test mpool_test_t;

    mpool_test_t check_list[] = {
    /*************** memory block base address is NULL ************/
    {NULL, 16, 16, 0},
    /**************** block count is 0 ********************/
    {mpool, 0, 10,0},
    /**************** block size is 0 ********************/
    {mpool, 1, 0,0},
    /**************** Normal setting **********************/
    {mpool, 16, 16, 1},
    /*yqy************ block count is negative number **********/
    {mpool,-1,10,0},
    /*yqy************ block size is negative number **********/
    {mpool,16,-1,0},
    };

    for(i=0;i<sizeof(check_list)/sizeof(mpool_test_t);i++)
    {
		next_test_case_notify();
        handle = csi_kernel_mpool_new(check_list[i].mpool,check_list[i].count, check_list[i].size);
		next_test_case_notify();
        if(check_list[i].equal) {
			MYASSERT(handle != NULL);
			if(handle != NULL)
			{
				test_case_success++;
				PRINT_RESULT("mpool_new_interface", PASS);
			}
			else
			{
				test_case_fail++;
				PRINT_RESULT("mpool_new_interface",FAIL);
			}
		}
        else {
			MYASSERT(handle == NULL);
			if(handle == NULL)
			{
				test_case_success++;
				PRINT_RESULT("mpool_new_interface",PASS);
			}
			else
			{
				test_case_fail++;
				PRINT_RESULT("mpool_new_interface",FAIL);
			}
		}
    }

	next_test_case_wait();
}
