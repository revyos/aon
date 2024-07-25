/*******************************************************
 *  CSI-ROTS Test Case --  msgq basic function test
 *  message size count test
 *******************************************************/

#include "msgq_test.h"
#include <string.h>
#include "dtest.h"
#include "test_util.h"


static k_msgq_handle_t handle;

void msgq_func_test()
{
    int i,count;
    k_status_t ret;
    handle = csi_kernel_msgq_new(5 ,20);
    char *msg[] = {"Hello\0","World\0", "test\0", "fin\0","get\0"};

    char *out;
    uint32_t rv;
	uint8_t v1 = 1;

	next_test_case_notify();
    rv = csi_kernel_msgq_get_capacity(NULL);
	BUFQUEUE_VAL_CHK(rv == 0);
	if(rv == 0)
	{
		test_case_success++;
		PRINT_RESULT("msgq_func_test",PASS);
	}
	else
	{
		test_case_fail++;
		PRINT_RESULT("msgq_func_test",FAIL);
	}

	next_test_case_notify();
    rv = csi_kernel_msgq_get_msg_size(NULL);
	BUFQUEUE_VAL_CHK(rv == 0);
	if(rv == 0)
	{
		test_case_success++;
		PRINT_RESULT("msgq_func_test",PASS);
	}
	else
	{
		test_case_fail++;
		PRINT_RESULT("msgq_func_test",FAIL);
	}

	next_test_case_notify();
    rv = csi_kernel_msgq_get_capacity(handle);
	BUFQUEUE_VAL_CHK(rv == 5);
	if(rv == 5)
	{
		test_case_success++;
		PRINT_RESULT("msgq_func_test",PASS);
	}
	else
	{
		test_case_fail++;
		PRINT_RESULT("msgq_func_test",FAIL);
	}

	next_test_case_notify();
    rv = csi_kernel_msgq_get_msg_size(handle);
#if CONFIG_KERNEL_FREERTOS
	BUFQUEUE_VAL_CHK(rv == 0);
	if(rv == 0)
	{
		test_case_success++;
		PRINT_RESULT("msgq_func_test",PASS);
	}
	else
	{
		test_case_fail++;
		PRINT_RESULT("msgq_func_test",FAIL);
	}
#else
	BUFQUEUE_VAL_CHK(rv == 20);
	if(rv == 20)
	{
		test_case_success++;
	    PRINT_RESULT("msgq_func_test",PASS);
	}
	else
	{
		test_case_fail++;
		PRINT_RESULT("msgq_func_test",FAIL);
	}    
#endif
    /*********** Fill message queue  ***************/
    for(i=0; i<5;i++)
    {
        ret = csi_kernel_msgq_put(handle,msg[i],0,0);
		if (ret != 0)
			v1 = 0;

    }

	next_test_case_notify();
	BUFQUEUE_VAL_CHK(v1 == 1);
	if(v1 == 1)
	{
		test_case_success++;
		PRINT_RESULT("msgq_func_test",PASS);
	}
	else
	{
		test_case_fail++;
		PRINT_RESULT("msgq_func_test",FAIL);
	}

    /***********  Check  message data *************/
    for(i=0;i<5; i++)
    {
		next_test_case_notify();
        out = (char *)malloc(20);
        ret =csi_kernel_msgq_get(handle, out, 0);
		BUFQUEUE_VAL_CHK(ret == 0);
		if(ret == 0)
		{
			test_case_success++;
			PRINT_RESULT("msgq_func_test",PASS);
		}
		else
		{
			test_case_fail++;
			PRINT_RESULT("msgq_func_test",FAIL);
		}

		next_test_case_notify();
		BUFQUEUE_VAL_CHK(memcmp(out, msg[i], strlen(msg[i])) == 0);
		if(memcmp(out, msg[i], strlen(msg[i])) == 0)
		{
			test_case_success++;
			PRINT_RESULT("msgq_func_test",PASS);
		}
		else
		{
			test_case_fail++;
			PRINT_RESULT("msgq_func_test",FAIL);
		}

		next_test_case_notify();
        count = csi_kernel_msgq_get_count(handle);
		BUFQUEUE_VAL_CHK(count == 4-i);
		if(count == 4-i)
		{
			test_case_success++;
			PRINT_RESULT("msgq_func_test",PASS);
		}
		else
		{
			test_case_fail++;
			PRINT_RESULT("msgq_func_test",FAIL);
		}
        free(out);
    }

	next_test_case_wait();
}

