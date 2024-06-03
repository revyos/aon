/*******************************************************
 *  CSI-ROTS Test Case --  semaphore basic function test
 *******************************************************/

#include "sem_test.h"


static k_sem_handle_t handle;
static k_task_handle_t task1, task2;


void sem_post()
{
    k_status_t ret;
    while(1)
    {
        printf("This is postfunc, now sem_count is %d\n", csi_kernel_sem_get_count(handle));
        ret = csi_kernel_sem_post(handle);

        if(ret != 0)
        {
            printf("exit post1\n");
            break;
        }
        csi_kernel_delay(100);
    }
    /************  This shall never entered  *******************/
    printf("sem_post task exited\n");
    csi_kernel_task_del(task1);
}

void sem_wait()
{
    k_status_t ret;
    while(1)
    {
        printf("This is waitfunc, now sem_count is %d\n", csi_kernel_sem_get_count(handle));
        ret = csi_kernel_sem_wait(handle,-1);
        if(ret != 0)
            break;
        ret = csi_kernel_sem_wait(handle,-1);
        if(ret != 0)
            break;
        csi_kernel_delay(100);
    } 

    /************  This shall never entered  *******************/
    printf("sem_wait task exited.\n");
    csi_kernel_task_del(task2);
}

static char post_stack[CSI_TEST_SEM_STACK];
static char wait_stack[CSI_TEST_SEM_STACK];

void sem_func_test3()
{
    handle = csi_kernel_sem_new(10, 5);
    
    csi_kernel_task_new((void *)&sem_post, "sem_post", NULL, 
          CSI_TEST_SEM_PRIO,0,post_stack,CSI_TEST_SEM_STACK, &task1);
    csi_kernel_task_new((void *)&sem_wait, "sem_wait", NULL, 
              CSI_TEST_SEM_PRIO,0,wait_stack,CSI_TEST_SEM_STACK,&task2);


    csi_kernel_task_del(csi_kernel_task_get_cur());

}


