/*******************************************************
 *  CSI-ROTS Test Case --  csi_kernel_msgq_get interface test
 *******************************************************/

#include "msgq_test.h"
#include "dtest.h"
#include "test_util.h"


void msgq_get_interface_test()
{
    int i;
    k_status_t ret;
    k_msgq_handle_t handle0;
    struct msgq_get_check {
        handle_status_check_t check;
        char *msg;
        int    timeout;
    };

    typedef struct msgq_get_check msgq_get_check_t;
    char *msg;
    handle0 = csi_kernel_msgq_new(3,5);
    msg = (char *) malloc(5);


    msgq_get_check_t check_list[] = {
    /**************** msgq_handle is NULL ********************/
    {{NULL, 0,0},msg, 0},
    /*************** msg is NULL ***********************/
    {{handle0, 0, 0}, NULL,0},
    /*************** Normal setting ***********************/
    {{handle0, 0, 1}, msg, 0},
    {{handle0, 0, 1}, msg, 10},
    {{handle0, 0, 1}, msg,-1},
    /*************** get msg when msgq is empty ***********************/
    {{handle0, 0, 0}, msg,10},
    {{handle0, 0, 0}, msg,0},
    };

    csi_kernel_msgq_put(handle0, "test\0", 0, 0);
    csi_kernel_msgq_put(handle0,"1234\0",0,0);
    csi_kernel_msgq_put(handle0,"we34\0",0,0);


    for(i=0;i<sizeof(check_list)/sizeof(msgq_get_check_t);i++)
    {
		next_test_case_notify();
        ret = csi_kernel_msgq_get(check_list[i].check.handle, check_list[i].msg,check_list[i].timeout);
        if(check_list[i].check.equal) {
			BUFQUEUE_VAL_CHK(check_list[i].check.status == ret);
			if(check_list[i].check.status == ret)
			{
				test_case_success++;
				PRINT_RESULT("msgq_get_interface",PASS);
			}
			else
			{
				test_case_fail++;
				PRINT_RESULT("msgq_get_interface", FAIL);
			}
		}
        else {
			BUFQUEUE_VAL_CHK(check_list[i].check.status != ret);
			if(check_list[i].check.status != ret)
			{
				test_case_success++;
				PRINT_RESULT("msgq_get_interface",PASS);
			}
			else
			{
				test_case_fail++;
				PRINT_RESULT("msgq_get_interface", FAIL);
			}
		}
    }

    free(msg);

	next_test_case_wait();
}
