/***************************************************************************
 *  CSI-ROTS Test Case --  msgq basic function test
 *  first put 1 message, then front put 1 message, get first message
 **************************************************************************/

#include "msgq_test.h"
#include <string.h>
#include "dtest.h"
#include "test_util.h"

static k_msgq_handle_t handle;

void msgq_func_test3()
{
    k_status_t ret;
    handle = csi_kernel_msgq_new(2,10);

    char *out;

    /************** Test front put  **********************/
	next_test_case_notify();
    ret = csi_kernel_msgq_put(handle, "Hello", 0,0);
	BUFQUEUE_VAL_CHK(ret == 0);
	if(ret == 0)
	{
		test_case_success++;
		PRINT_RESULT("msgq_func_test3", PASS);
	}
	else
	{
		test_case_fail++;
		PRINT_RESULT("msgq_func_test3",FAIL);
	}

	next_test_case_notify();
    ret = csi_kernel_msgq_put(handle, "World", 1,0);
	BUFQUEUE_VAL_CHK(ret == 0);
	if(ret == 0)
	{
		test_case_success++;
		PRINT_RESULT("msgq_func_test3", PASS);
	}
	else
	{
		test_case_fail++;
		PRINT_RESULT("msgq_func_test3",FAIL);
	}

	next_test_case_notify();
    out = (char *)malloc(10);
    ret = csi_kernel_msgq_get(handle, out, 0); 
	BUFQUEUE_VAL_CHK(ret == 0);
	if(ret == 0)
	{   
		test_case_success++;
		PRINT_RESULT("msgq_func_test3", PASS);
	}
	else
	{   
		test_case_fail++;
		PRINT_RESULT("msgq_func_test3",FAIL);
	}
   
	next_test_case_notify();
#if CONFIG_KERNEL_RHINO
	BUFQUEUE_VAL_CHK(memcmp(out,"Hello",5) == 0);
	if(memcmp(out,"Hello",5) == 0)
	{
		test_case_success++;
		PRINT_RESULT("msgq_func_test3", PASS);
	}
	else
	{
		test_case_fail++;
		PRINT_RESULT("msgq_func_test3",FAIL);
	}
#else
	BUFQUEUE_VAL_CHK(memcmp(out,"World",5) == 0);
	if(memcmp(out,"World",5) == 0)
	{
		test_case_success++;
		PRINT_RESULT("msgq_func_test3", PASS);
	}
	else
	{
		test_case_fail++;
		PRINT_RESULT("msgq_func_test3",FAIL);
	}
#endif
    free(out);

    /***************  Test msgq_flush  ***************/
	next_test_case_notify();
    out = (char *)malloc(10);
    ret = csi_kernel_msgq_put(handle, "World", 0,0);
	BUFQUEUE_VAL_CHK(ret == 0);
	if(ret == 0)
	{
		test_case_success++;
		PRINT_RESULT("msgq_func_test3", PASS);
	}
	else
	{
		test_case_fail++;
		PRINT_RESULT("msgq_func_test3",FAIL);
	}
	
	next_test_case_notify();
	ret = csi_kernel_msgq_flush(handle);
	BUFQUEUE_VAL_CHK(ret == 0);
	if(ret == 0)
	{
		test_case_success++;
		PRINT_RESULT("msgq_func_test3", PASS);
	}
	else
	{
		test_case_fail++;
		PRINT_RESULT("msgq_func_test3",FAIL);
	}
   
	next_test_case_notify();
    ret = csi_kernel_msgq_get(handle, out, 0);
	BUFQUEUE_VAL_CHK(ret != 0);
	if(ret != 0)
	{
		test_case_success++;
		PRINT_RESULT("msgq_func_test3", PASS);
	}
	else
	{
		test_case_fail++;
		PRINT_RESULT("msgq_func_test3",FAIL);
	}

    free(out);

	next_test_case_wait();
}


