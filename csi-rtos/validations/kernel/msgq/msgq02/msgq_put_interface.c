/*******************************************************
 *  CSI-ROTS Test Case --  csi_kernel_msgq_put interface test
 *******************************************************/

#include "msgq_test.h"
#include "dtest.h"
#include "test_util.h"


void msgq_put_interface_test()
{
    int i;
    k_status_t ret;
    k_msgq_handle_t handle0;
    struct msgq_put_check {
        handle_status_check_t check;
        char *msg;
        uint8_t fob;
        int    timeout;
    };
    handle0 = csi_kernel_msgq_new(4,5);

    struct msgq_put_check check_list[] = {
    /**************** msgq_handle is NULL ********************/
    {{NULL, 0,0},"test\0", 0, 0},
    /*************** msg is NULL ***********************/
    {{handle0, 0, 0}, NULL, 0,0},
    /**************** fob larger than 1 ****************************/
    {{handle0, 0, 0}, "test\0", 2, 0},
    /*************** Normal settings ***************************/
    {{handle0, 0, 1}, "test\0", 0, 0}, //inserted into the tail of messge_queue
    {{handle0, 0, 1}, "tet\0",  1, 0}, //inserted into the head of messge_queue
    {{handle0, 0, 1}, "tes\0",  0, 5}, //timeout is 5
    {{handle0, 0, 1}, "tst\0",  1,-1}, //timeout is negative number
    /************** msgq is full *******************************/
    {{handle0, 0, 0}, "test\0", 0,0 },
    };


    for(i=0;i<sizeof(check_list)/sizeof(struct msgq_put_check);i++)
    {
		next_test_case_notify();
        ret = csi_kernel_msgq_put(check_list[i].check.handle, check_list[i].msg, 
                           check_list[i].fob, check_list[i].timeout);
        if(check_list[i].check.equal) {
			BUFQUEUE_VAL_CHK(check_list[i].check.status == ret);
			if(check_list[i].check.status == ret)
			{
				test_case_success++;
				PRINT_RESULT("msgq_put_interface", PASS);
			}
			else
			{
				test_case_fail++;
				PRINT_RESULT("msgq_put_interface", FAIL);
			}
		}
        else {
			BUFQUEUE_VAL_CHK(check_list[i].check.status != ret);
			if(check_list[i].check.status != ret)
			{
				test_case_success++;
				PRINT_RESULT("msgq_put_interface", PASS);
			}
			else
			{
				test_case_fail++;
				PRINT_RESULT("msgq_put_interface", FAIL);
			}
		}
    }

	next_test_case_wait();
}
