/*******************************************************
 *  CSI-ROTS Test Case --  semaphore basic function test
 *******************************************************/

#include "sem_test.h"
#include "dtest.h"
#include "test_util.h"

void sem_func_test()
{
    k_sem_handle_t handle;
    int i;
    k_status_t ret;
	int32_t count;

    handle = csi_kernel_sem_new(10, 0);
    
    for(i=1;i<11;i++)
    {
		next_test_case_notify();
        ret = csi_kernel_sem_post(handle);
		MYASSERT(ret == 0);
		if(ret == 0)
		{
			test_case_success++;
			PRINT_RESULT("sem_func_test",PASS);
		}
		else
		{
			test_case_fail++;
			PRINT_RESULT("sem_func_test",FAIL);
		}
		count = csi_kernel_sem_get_count(handle);
		next_test_case_notify();
		MYASSERT(count == i);
		if(count == i)
		{
			test_case_success++;
			PRINT_RESULT("sem_func_test",PASS);
		}
		else
		{
			test_case_fail++;
			PRINT_RESULT("sem_func_test",FAIL);
		}
    }

	next_test_case_notify();
    ret = csi_kernel_sem_post(handle);
#if CONFIG_KERNEL_FREERTOS
	MYASSERT(ret != 0);
	if(ret !=0)
	{
		test_case_success++;
		PRINT_RESULT("sem_func_test",PASS);
	}
	else
	{
		test_case_fail++;
		PRINT_RESULT("sem_func_test",FAIL);
	}
#else
	MYASSERT(ret == 0);
	if(ret ==0)
	{
		test_case_success++;
		PRINT_RESULT("sem_func_test",PASS);
	}
	else
	{
		test_case_fail++;
		PRINT_RESULT("sem_func_test",FAIL);
	}
#endif

	next_test_case_wait();
}
