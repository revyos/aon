/*******************************************************
 *  CSI-ROTS Test Case --  csi_kernel_task_new interface test
 *******************************************************/

#include "sem_test.h"
#include "dtest.h"
#include "test_util.h"


void sem_new_interface_test()
{
    int i;
    uint32_t check_list[][3] = {
    /****************** max_count and init_count are all 0  ****************************/
    {0 ,0 , 0},
    /****************** max_count less than init_count   ***************************/
    {4, 5, 0},
    /****************** max_count is negative number   ***************************/
    {-1, 0, 0},
    /****************** init_count is negative number   ***************************/
    {4, -1, 0},
    /********* max_count and init_count are all negative number  ***********************/
    {-3 ,-1 , 0},
    /****************** max_count and init_count are all 0x7fffffff ****************/
    {0x7fffffff, 0x7fffffff, 1},
    };
    int size = sizeof(check_list)/(sizeof(uint32_t)*3);
    k_sem_handle_t  handle;
    
    for(i=0;i<size;i++)
    {
		next_test_case_notify();
        handle = csi_kernel_sem_new(check_list[i][0],check_list[i][1]);
        if(check_list[i][2]) {
			MYASSERT(handle != NULL);
			if(handle != NULL)
			{
				test_case_success++;
				PRINT_RESULT("sem_new_interface", PASS);
			}
			else
			{
				test_case_fail++;
				PRINT_RESULT("sem_new_interface", FAIL);
			}
		}
        else {
			MYASSERT(handle == NULL);
			if(handle == NULL)
			{
				test_case_success++;
				PRINT_RESULT("sem_new_interface", PASS);
			}
			else
			{
				test_case_fail++;
				PRINT_RESULT("sem_new_interface", FAIL);
			}
		}
    }

	next_test_case_wait();
}
