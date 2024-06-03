/*******************************************************
 *  CSI-ROTS Test Case --  csi_kernel_task_set_prio interface test
 *******************************************************/

#include "task_test.h"
#include "dtest.h"
#include "test_util.h"

void task_set_prio_interface_test()
{
    typedef struct set_prio {
        handle_status_check_t   check;
        k_priority_t            prio;
    } set_prio_t;

    set_prio_t check_list[] = {
    /**************** handle is NULL ****************/
    {{NULL,0,0}, CSI_TEST_TASK_MIN_PRIO},
    /**************** handle is not available ******/
    //{{handle,0,0}, CSI_TEST_TASK_MIN_PRIO},
    /**************** handle is not a handle ******/
    //{{set_prio_if_stack, 0,0},CSI_TEST_TASK_MIN_PRIO},//refer to JIRA issue CSIR-65,this issue won't fix,so diable it.
    };
    
    k_status_t ret;
    int i;
    
    for(i=0; i<sizeof(check_list)/sizeof(set_prio_t);i++)
    {
		next_test_case_notify();
        ret = csi_kernel_task_set_prio(check_list[i].check.handle, check_list[i].prio);
        if (check_list[i].check.equal) {
			MYASSERT(check_list[i].check.status == ret);
			if(check_list[i].check.status == ret)
			{
				test_case_success++;
				PRINT_RESULT("task_set_prio_interface", PASS);
			}
			else
			{
				test_case_fail++;
				PRINT_RESULT("task_set_prio_interface", FAIL);
			}
        } else {
			MYASSERT(check_list[i].check.status != ret);
			if(check_list[i].check.status != ret)
			{
				test_case_success++;
				PRINT_RESULT("task_set_prio_interface", PASS);
			}
			else
			{
				test_case_fail++;
				PRINT_RESULT("task_set_prio_interface", FAIL);
			}
        }
    }

	next_test_case_wait();
}
   
