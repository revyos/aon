/*******************************************************
 *  CSI-ROTS Test Case --  mpool basic function test
 *  message size count test
 *******************************************************/

#include "mpool_test.h"
#include <string.h>
#include "dtest.h"
#include "test_util.h"

static k_mpool_handle_t handle;

//const char *casename = "mpool_func";

static char mpool[256];

void mpool_func_test()
{
    int i,j;
    k_status_t ret;
    void *block[4];
    uint32_t rv;
	int32_t active_num;

	next_test_case_notify();
    handle = csi_kernel_mpool_new(mpool, 4,64);

    rv = csi_kernel_mpool_get_capacity(NULL);
	MYASSERT(rv == 0);
	if(rv == 0)
	{
		test_case_success++;
		PRINT_RESULT("mpool_func", PASS);
	}
	else
	{
		test_case_fail++;
		PRINT_RESULT("mpool_func", FAIL);
	}

	next_test_case_notify();
    rv = csi_kernel_mpool_get_block_size(NULL);
	MYASSERT(rv == 0);
	if(rv == 0)
	{
		test_case_success++;
		PRINT_RESULT("mpool_func", PASS);
	}
	else
	{
		test_case_fail++;
		PRINT_RESULT("mpool_func", FAIL);
	}

    for(j=0;j<3;j++)
    {

        //printf("Begin to Allocate memory\n");

        /*********** Alloc blocks and use them  ***************/
        for(i=0; i<4;i++)
        {  
			next_test_case_notify();
            block[i] = csi_kernel_mpool_alloc(handle,0);
			MYASSERT(block[i] != NULL);
			if(block[i] != NULL)
			{
				test_case_success++;
				PRINT_RESULT("mpool_func", PASS);
			}
			else
			{
				test_case_fail++;
				PRINT_RESULT("mpool_func", FAIL);
			}
			
			next_test_case_notify();
			active_num = csi_kernel_mpool_get_count(handle);
			MYASSERT(active_num == i+1);
			if(active_num == i+1)
			{
				test_case_success++;
				PRINT_RESULT("mpool_func", PASS);
			}
			else
			{
				test_case_fail++;
				PRINT_RESULT("mpool_func", FAIL);
			}
	    	//ASSERT(block[i] != NULL);
            //printf("Now there are(is) %d block(s) available.\n", csi_kernel_mpool_get_count(handle));
            /*************  Fill allocated memory with 0x0 to check if it is available ****/
            memset(block[i], 0x0,64);
        }
		next_test_case_notify();
        rv = csi_kernel_mpool_get_capacity(handle);
		MYASSERT(rv == 4);
		if(rv == 4)
		{
			test_case_success++;
			PRINT_RESULT("mpool_func", PASS);
		}
		else
		{
			test_case_fail++;
			PRINT_RESULT("mpool_func", FAIL);
		}
		
		next_test_case_notify();
        rv = csi_kernel_mpool_get_block_size(handle);
		MYASSERT(rv == 64);
		if(rv == 64)
		{
			test_case_success++;
			PRINT_RESULT("mpool_func", PASS);
		}
		else
		{
			test_case_fail++;
			PRINT_RESULT("mpool_func", FAIL);
		}

        //printf("Begin to free memory\n");
        /***********  Check  message data *************/
        for(i=0;i<4; i++)
        {
			next_test_case_notify();
            ret = csi_kernel_mpool_free(handle,block[i]);
			MYASSERT(ret == 0);
			if(ret == 0)
			{
				test_case_success++;
				PRINT_RESULT("mpool_func", PASS);
			}
			else
			{
				test_case_fail++;
				PRINT_RESULT("mpool_func", FAIL);
			}
			
			next_test_case_notify();
            active_num = csi_kernel_mpool_get_count(handle);
			MYASSERT(active_num == 3-i);
			if(active_num == 3-i)
			{
				test_case_success++;
				PRINT_RESULT("mpool_func", PASS);
			}
			else
			{
				test_case_fail++;
				PRINT_RESULT("mpool_func", FAIL);
			}
            //printf("Now there are(is) %d block(s) available.\n", csi_kernel_mpool_get_count(handle));
        }

		next_test_case_notify();
        rv = csi_kernel_mpool_get_capacity(handle);
		MYASSERT(rv == 4);
		if(rv == 4)
		{
			test_case_success++;
			PRINT_RESULT("mpool_func", PASS);
		}
		else
		{
			test_case_fail++;
			PRINT_RESULT("mpool_func", FAIL);
		}
    }

	next_test_case_wait();
}

