/*******************************************************
 *  CSI-ROTS Test Case --  semaphore basic function test
 *******************************************************/

#include "sem_test.h"
#include "dtest.h"
#include "test_util.h"


void sem_func_test2()
{
    k_sem_handle_t handle;

    int i;
    k_status_t ret;
    handle = csi_kernel_sem_new(10, 10);
	int32_t count;

    for(i=9;i>=0;i--)
    {
		next_test_case_notify();
        ret = csi_kernel_sem_wait(handle,0);
		MYASSERT(ret == 0);
		if(ret == 0)
		{
			test_case_success++;
		    PRINT_RESULT("sem_func_test2",PASS);
	    }
		else
		{
			test_case_fail++;
			PRINT_RESULT("sem_func_test2",FAIL);
		}
		next_test_case_notify();
		count = csi_kernel_sem_get_count(handle);
		MYASSERT(count == i);
		if(count == i)
		{
			test_case_success++;
			PRINT_RESULT("sem_func_test2",PASS);
		}
		else
		{
			test_case_fail++;
			PRINT_RESULT("sem_func_test2",FAIL);
		}
    }
    
	next_test_case_notify();
    ret = csi_kernel_sem_wait(handle,0);
	MYASSERT(ret != 0);
	if(ret !=0)
	{
		test_case_success++;
		PRINT_RESULT("sem_func_test2",PASS);
	}
	else
	{
		test_case_fail++;
		PRINT_RESULT("sem_func_test2",FAIL);
	}

	next_test_case_wait();

}
