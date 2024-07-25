/*******************************************************
 *  CSI-ROTS Test Case --   get task account and delete test
 *******************************************************/

#include "task_test.h"
#include <stdio.h>

uint8_t check = 2;
extern k_sem_handle_t g_sem_handle;
extern uint32_t total_count;

static void test_func()
{
    check--;
    while(check);
    
    csi_kernel_task_del(csi_kernel_task_get_cur());
}


void task_quanta_test()
{
    k_task_handle_t handle, handle1;
    total_count +=1;

    csi_kernel_task_new(test_func, "test_task", NULL, CSI_TEST_TASK_MIN_PRIO, 1,
                          NULL, CSI_TEST_TASK_STACK, &handle);
    csi_kernel_task_new(test_func,"test_task", NULL, CSI_TEST_TASK_MIN_PRIO, 1,
                          NULL, CSI_TEST_TASK_STACK, &handle1);

    csi_kernel_delay(1);

    while(check);

    printf("Task QuantaTest finished\n");
    csi_kernel_sem_post(g_sem_handle);
    csi_kernel_task_del(csi_kernel_task_get_cur());
}
