/*
 * Copyright (C) 2017-2019 Alibaba Group Holding Limited
 */

/******************************************************************************
 * @file     timer02.c
 * @brief    timer driver test
 * @version  V1.0
 * @date     14. Jun 2019
 ******************************************************************************/

#include "timer_test.h"
#include <stdio.h>
#include "dtest.h"

static uint8_t intr_count;

void timer_event_cb(int32_t idx, timer_event_e event)
{
    if (event == TIMER_EVENT_TIMEOUT) {
        printf("Timer%d event entered!\n", idx);
        intr_count = 0;
    }
}

void timer_if_test()
{
    //initialize
    timer_handle_t timer = csi_timer_initialize(5, timer_event_cb);
    ASSERT_TRUE(timer == NULL);

    timer = csi_timer_initialize(0, NULL);
    ASSERT_TRUE(timer != NULL);

    int32_t ret = 0;

    ret = csi_timer_power_control(NULL, DRV_POWER_FULL);
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_TIMER_BASE | DRV_ERROR_PARAMETER));
    //ASSERT_TRUE(ret == (CSI_DRV_ERRNO_TIMER_BASE |DRV_ERROR_UNSUPPORTED));

    ret = csi_timer_power_control(timer, 6);
    //ASSERT_TRUE(ret == 0);
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_TIMER_BASE | DRV_ERROR_UNSUPPORTED)); //CM6501 UNSUPPORTED

    //uninitialize
    ret = csi_timer_uninitialize(NULL);
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_TIMER_BASE | DRV_ERROR_PARAMETER));

    //config
    ret = csi_timer_config(NULL, TIMER_MODE_RELOAD);
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_TIMER_BASE | DRV_ERROR_PARAMETER));

    ret = csi_timer_config(timer, 2);
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_TIMER_BASE | DRV_ERROR_PARAMETER));

    ret = csi_timer_config(timer, TIMER_MODE_RELOAD);

    //set_timeout
    ret = csi_timer_set_timeout(NULL, 1000000);
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_TIMER_BASE | DRV_ERROR_PARAMETER));

    //start
    ret = csi_timer_start(NULL);
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_TIMER_BASE | DRV_ERROR_PARAMETER));

    uint32_t value;
    //get_current_value
    ret = csi_timer_get_current_value(NULL, &value);
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_TIMER_BASE | DRV_ERROR_PARAMETER));

    ret = csi_timer_get_current_value(timer, NULL);
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_TIMER_BASE | DRV_ERROR_PARAMETER));

    //get_status
    timer_status_t stat = csi_timer_get_status(NULL);
    ASSERT_TRUE(stat.active == 0);

    //get_reload_value
    ret = csi_timer_get_load_value(NULL, &value);
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_TIMER_BASE | DRV_ERROR_PARAMETER));

    ret = csi_timer_get_load_value(timer, NULL);
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_TIMER_BASE | DRV_ERROR_PARAMETER));

    //stop
    ret = csi_timer_stop(NULL);
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_TIMER_BASE | DRV_ERROR_PARAMETER));

    //suspend
    ret = csi_timer_suspend(NULL);
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_TIMER_BASE | DRV_ERROR_PARAMETER));

    //resume
    ret = csi_timer_resume(NULL);
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_TIMER_BASE | DRV_ERROR_PARAMETER));

    csi_timer_uninitialize(timer);
}

void timer_func_test()
{
    int i = 0, j, count;
    uint32_t ret;
    char *out;
    int size;
    timer_handle_t timer[TIMER_TOTLE_COUNT];
    timer_mode_e mode = TIMER_MODE_RELOAD;

    intr_count = 1;

    /*******  Test get instance  *********/
    for (i = 0; i < TIMER_TOTLE_COUNT; i++) {
        timer[i] = csi_timer_initialize(i, timer_event_cb);

        if (timer[i] == NULL) {
            printf("Fail to init timer, this test failed cannot be continued!\n");
            return;
        }
    }

    uint32_t out_value = 0, out_value1 = 0, out_value2 = 0, out_value3 = 0;

    /*******************************************
     *  Case-0 Use timer[0]
     *************************************************/
#if 0
    i = TEST_TIMER;
    ret = csi_timer_config(timer[i], mode);
    ASSERT_TRUE(ret == 0);
    ret = csi_timer_set_timeout(timer[i], TIMER_TEST_DURATION);//20s
    ASSERT_TRUE(ret == 0);

    //ret = csi_timer_get_load_value(timer[i], &out_value);
    //printf("load value is 0x%x\n",out_value);

    //printf("Timer%d start.\n",i);
    ret = csi_timer_start(timer[i]);
    ASSERT_TRUE(ret == 0);

    ret = csi_timer_get_current_value(timer[i], &out_value);
    ASSERT_TRUE(ret == 0);
    ret = csi_timer_get_load_value(timer[i], &out_value1);
    ASSERT_TRUE(ret == 0);

    //mdelay(TIMER_TEST_DURATION/1000);
    while (intr_count) ;

    intr_count = 1;

    mdelay(1000);
    //printf("Timer%d timeout\n",i);
    //printf("Timer0 should timeout, and callback function should be called.\n");
    ret = csi_timer_stop(timer[i]);
    ASSERT_TRUE(ret == 0);

    ret = csi_timer_get_current_value(timer[i], &out_value3);
    ASSERT_TRUE(ret == 0);
    ASSERT_TRUE(out_value3 != out_value);

    ret = csi_timer_get_load_value(timer[i], &out_value2);
    ASSERT_TRUE(ret == 0);
    ASSERT_TRUE(out_value1 == out_value2);

    //printf("out_value1:	%d\nout_value2:	%d\nout_value3:	%d\nout_value:	%d\n",out_value1,out_value2,out_value3,out_value);

    mdelay(2000);
    ret = csi_timer_get_current_value(timer[i], &out_value);
    ASSERT_TRUE(ret == 0);
    ASSERT_TRUE(out_value3 == out_value);
    csi_timer_get_load_value(timer[i], &out_value2);
    //printf("out_value1: %d\nout_value2: %d\nout_value3: %d\nout_value:  %d\n",out_value1,out_value2,out_value3,out_value);
    ASSERT_TRUE(out_value1 == out_value2);

//sc5654a don't supported csi_timer_resume()
    /*
    	ret = csi_timer_resume(timer[i]);
    	ASSERT_TRUE(ret == 0);
    	while(intr_count) ;
    */

    ret = csi_timer_stop(timer[i]);
    ASSERT_TRUE(ret == 0);

#endif

    /***********************************
     * Case-1 Use all timers
      **********************************/
#if 1
    /**************  First init and config all the timers   ****************************/

    for (i = 0; i < TIMER_TOTLE_COUNT; i++) {
        ret = csi_timer_config(timer[i], TIMER_MODE_FREE_RUNNING);
        ASSERT_TRUE(ret == 0);
        ret = csi_timer_set_timeout(timer[i], TIMER_TEST_DURATION);
        ASSERT_TRUE(ret == 0);
    }

    for (i = 0; i < TIMER_TOTLE_COUNT; i++) {
        printf("Timer%d start.\n", i);
        ret = csi_timer_start(timer[i]);
        ASSERT_TRUE(ret == 0);
        mdelay(2000);
    }

    timer_status_t status;

    for (i = 0; i < TIMER_TOTLE_COUNT; i++) {
        status = csi_timer_get_status(timer[i]);
        ASSERT_TRUE(status.active == 1);
    }

    mdelay(60000);

    for (i = 0; i < TIMER_TOTLE_COUNT; i++) {
        printf("Timer%d should timeout, and callback function should be called.\n", i);
        ret = csi_timer_stop(timer[i]);
        ASSERT_TRUE(ret == 0);
        mdelay(1000);
    }

#endif
    /************************************************
     * Case-4 Change timeout settings
     **************************************************/
#if 0
    i = TEST_TIMER;
    //printf("Case4 starts here.\n");
    ret = csi_timer_config(timer[i], mode);
    ASSERT_TRUE(ret == 0);
    printf("Change timer timeout settings to %d seconds\n", TIMER_TEST_DURATION / 2 / 1000000);
    ret = csi_timer_set_timeout(timer[i], TIMER_TEST_DURATION / 2);
    ASSERT_TRUE(ret == 0);
    printf("Start timer%d\n", i);
    csi_timer_start(timer[i]);
    mdelay(TIMER_TEST_DURATION / 4 / 1000);
    printf("Now,Change timer timeout to %d seconds\n", TIMER_TEST_DURATION / 1000000);
    ret = csi_timer_set_timeout(timer[i], TIMER_TEST_DURATION);
    ASSERT_TRUE(ret == 0);
    ret = csi_timer_resume(timer[i]);
    ASSERT_TRUE(ret == 0);
    mdelay(TIMER_TEST_DURATION / 2 / 1000);

    printf("Timer shouldn't timeout\n");
    ret = csi_timer_stop(timer[i]);
    ASSERT_TRUE(ret == 0);
#endif

    /************************************************
     * Case-5 Timer pause and resume
     **************************************************/
    /*printf("Case5 starts here.\n");
    ret = csi_timer_set_timeout(timer[0], TIMER_TEST_DURATION);
    ASSERT_TRUE(ret == 0);
    printf("Start timer[0]\n");
    ret = csi_timer_start(timer[0]);
    ASSERT_TRUE(ret == 0);

    mdelay(TIMER_TEST_DURATION/1000/2);
    ret = csi_timer_suspend(timer[0]);
    ASSERT_TRUE(ret == 0);
    printf("Timer paused!\n");

    mdelay(TIMER_TEST_DURATION/1000/2);
    printf("Timer should not timeout.\n");

    printf("Then resume timer.\n");
    ret = csi_timer_resume(timer[0]);
    ASSERT_TRUE(ret == 0);

    mdelay(TIMER_TEST_DURATION/1000/2);
    printf("Timer0 should timeout, and callback function should be called.\n");
    ret = csi_timer_stop(timer[0]);
    ASSERT_TRUE(ret == 0);*/

    /************************************************
     * Case-6 Timeout is 0
     **************************************************/
    /*printf("Case6 starts here.\n");
    ret = csi_timer_set_timeout(timer[0], 0);
    ASSERT_TRUE(ret == 0);

    printf("Start timer[0]\n");
    ret = csi_timer_start(timer[0]);
    ASSERT_TRUE(ret == 0);

    mdelay(10);
    printf("Timer0 should timeout, and callback function should be called.\n");
    ret = csi_timer_stop(timer[0]);
    ASSERT_TRUE(ret == 0);*/

    /**************** Test Suite Teardown steps ******************/
    for (i = 0; i < TIMER_TOTLE_COUNT; i++) {
        ret = csi_timer_uninitialize(timer[i]);
        ASSERT_TRUE(ret == 0);
    }

}

void timer_func1_test()
{
    timer_handle_t timer;

    timer = csi_timer_initialize(2, NULL);

    uint32_t out_value = 0;

    csi_timer_config(timer, TIMER_MODE_RELOAD);
    csi_timer_set_timeout(timer, 20000000);
    csi_timer_start(timer);
    mdelay(1000);
    csi_timer_get_current_value(timer, &out_value);
    printf("%d\n", out_value);
    mdelay(3000);
    csi_timer_get_current_value(timer, &out_value);
    printf("%d\n", out_value);

    csi_timer_stop(timer);
    mdelay(1000);
    csi_timer_get_current_value(timer, &out_value);
    printf("%d\n", out_value);

    csi_timer_uninitialize(timer);
}

int test_timer(void)
{
    dtest_suite_info_t test_suite_info = {
        "test_timer", NULL, NULL, NULL, NULL
    };

    dtest_suite_t *test_suite = dtest_add_suite(&test_suite_info);

    dtest_case_info_t test_case_info_array[] = {
        {"timer_if_test", timer_if_test, 1},
        {"timer_func_test", timer_func_test, 1},
        {"timer_func1_test", timer_func1_test, 1},
        {NULL, NULL, 0}
    };

    dtest_add_cases(test_suite, test_case_info_array);

    return 0;
}

int main(void)
{
    dtest_register_cb(test_timer);
    dtest();

    return 0;
}
