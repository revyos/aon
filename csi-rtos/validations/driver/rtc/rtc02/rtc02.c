/*
 * Copyright (C) 2017-2019 Alibaba Group Holding Limited
 */

/******************************************************************************
 * @file     task02.c
 * @brief    the main function
 * @version  V1.0
 * @date     14. Jun 2019
 ******************************************************************************/

#include "rtc_test.h"
#include <stdio.h>
#include "dtest.h"

void rtc_event_cb()
{
    printf("Alarm!!!\n");
}

void check_rtc_time(struct tm cmp, uint8_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t min)
{
    ASSERT_TRUE(cmp.tm_year == year);
    ASSERT_TRUE(cmp.tm_mon == month);
    ASSERT_TRUE(cmp.tm_mday == day);
    ASSERT_TRUE(cmp.tm_hour == hour);
    ASSERT_TRUE(cmp.tm_min == min);
}

void rtc_if_test()
{
    //get_capabilities
    rtc_capabilities_t cap = csi_rtc_get_capabilities(2);
    ASSERT_TRUE(cap.interrupt_mode == 0);
    ASSERT_TRUE(cap.wrap_mode == 0);

    //initialize
    rtc_handle_t rtc = csi_rtc_initialize(1, rtc_event_cb);
    ASSERT_TRUE(rtc == NULL);

    rtc = csi_rtc_initialize(0, NULL);
    ASSERT_TRUE(rtc != NULL);

    int32_t ret;

    // power control
    ret = csi_rtc_power_control(NULL, DRV_POWER_FULL);
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_RTC_BASE | DRV_ERROR_UNSUPPORTED));
    ret = csi_rtc_power_control(rtc, 5);
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_RTC_BASE | DRV_ERROR_UNSUPPORTED));

    //uninitialize
    ret = csi_rtc_uninitialize(NULL);
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_RTC_BASE | DRV_ERROR_PARAMETER));

    //start
    ret = csi_rtc_start(NULL);
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_RTC_BASE | DRV_ERROR_PARAMETER));

    csi_rtc_start(rtc);

    struct tm set_time;
    set_time.tm_year   = 117;
    set_time.tm_mon    = 11;
    set_time.tm_mday   = 31;
    set_time.tm_hour   = 23;
    set_time.tm_min    = 59;
    set_time.tm_sec    = 59;
    //set_time
    ret = csi_rtc_set_time(NULL, &set_time);
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_RTC_BASE | DRV_ERROR_PARAMETER));

    ret = csi_rtc_set_time(rtc, NULL);
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_RTC_BASE | DRV_ERROR_PARAMETER));

    ret = csi_rtc_set_time(rtc, &set_time);

    //get_time
    ret = csi_rtc_get_time(NULL, &set_time);
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_RTC_BASE | DRV_ERROR_PARAMETER));

    ret = csi_rtc_get_time(rtc, NULL);
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_RTC_BASE | DRV_ERROR_PARAMETER));

    //set_alarm
    ret = csi_rtc_set_alarm(NULL, &set_time);
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_RTC_BASE | DRV_ERROR_PARAMETER));

    ret = csi_rtc_set_alarm(rtc, NULL);
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_RTC_BASE | DRV_ERROR_PARAMETER));

    //enable_alarm
    ret = csi_rtc_enable_alarm(NULL, 0);
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_RTC_BASE | DRV_ERROR_PARAMETER));

    //get_status
    rtc_status_t stat = csi_rtc_get_status(NULL);
    ASSERT_TRUE(stat.active == 0);

    //stop
    ret = csi_rtc_stop(NULL);
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_RTC_BASE | DRV_ERROR_PARAMETER));

    csi_rtc_stop(rtc);

    csi_rtc_uninitialize(rtc);

}

void rtc_func_test()
{
    int count;
    uint32_t ret;
    rtc_handle_t rtc;

    rtc_capabilities_t rtc_cap;
    struct tm set_time, get_time;

    /*******  Test get instance  *********/
    rtc = csi_rtc_initialize(0, rtc_event_cb);
    ASSERT_TRUE(rtc != NULL);

    rtc_cap = csi_rtc_get_capabilities(0);

    if (rtc_cap.interrupt_mode == 1) {
        printf("This RTC support interrupt mode\n");
    }

    if (rtc_cap.wrap_mode == 1) {
        printf("This RTC support wrap mode\n");
    }

    ret = csi_rtc_start(rtc);

    /*****************************************************************
     *  Case-0 Set time to 2017-12-31 23:59:59 and wait one second
     ****************************************************************/
    //printf("Case-0 starts here.\n");

    set_time.tm_year   = 117;
    set_time.tm_mon    = 11;
    set_time.tm_mday   = 31;
    set_time.tm_hour   = 23;
    set_time.tm_min    = 59;
    set_time.tm_sec    = 59;

    ret = csi_rtc_set_time(rtc, &set_time);
    ASSERT_TRUE(ret == 0);
    mdelay(2000);

    ret = csi_rtc_get_time(rtc, &get_time);
    ASSERT_TRUE(ret == 0);

    check_rtc_time(get_time, 118, 0, 1, 0, 0);

    rtc_status_t status;
    status = csi_rtc_get_status(rtc);
    ASSERT_TRUE(status.active == 1);
#if 1
    /*****************************************************************
     * Case-1 Set time to 2017-11-30 23:59:59 and wait one second
     ****************************************************************/

    //printf("Case-1 starts here.\n");

    set_time.tm_year   = 117;
    set_time.tm_mon    = 10;
    set_time.tm_mday   = 30;
    set_time.tm_hour   = 23;
    set_time.tm_min    = 59;
    set_time.tm_sec    = 59;

    ret = csi_rtc_set_time(rtc, &set_time);
    ASSERT_TRUE(ret == 0);
    mdelay(2000);

    ret = csi_rtc_get_time(rtc, &get_time);
    ASSERT_TRUE(ret == 0);

    check_rtc_time(get_time, 117, 11, 1, 0, 0);

    /************************************************************
     * Case-2 Set time to 2017-2-28 23:59:59 and wait one second
     ***********************************************************/
    //printf("Case2 starts here.\n");

    set_time.tm_year   = 117;
    set_time.tm_mon    = 1;
    set_time.tm_mday   = 28;
    set_time.tm_hour   = 23;
    set_time.tm_min    = 59;
    set_time.tm_sec    = 59;

    ret = csi_rtc_set_time(rtc, &set_time);
    ASSERT_TRUE(ret == 0);
    mdelay(2000);

    ret = csi_rtc_get_time(rtc, &get_time);
    ASSERT_TRUE(ret == 0);

    check_rtc_time(get_time, 117, 2, 1, 0, 0);

    /*************************************************************
     * Case-3 Set time to 2016-2-28 23:59:59 and wait one second
     ************************************************************/
    //printf("Case3 starts here.\n");

    set_time.tm_year   = 116;
    set_time.tm_mon    = 1;
    set_time.tm_mday   = 28;
    set_time.tm_hour   = 23;
    set_time.tm_min    = 59;
    set_time.tm_sec    = 59;

    ret = csi_rtc_set_time(rtc, &set_time);
    ASSERT_TRUE(ret == 0);
    mdelay(2000);

    ret = csi_rtc_get_time(rtc, &get_time);
    ASSERT_TRUE(ret == 0);

    check_rtc_time(get_time, 116, 1, 29, 0, 0);

    /*************************************************************
     * Case-4 Set time to 2016-2-29 23:59:59 and wait one second
     ************************************************************/
    //printf("Case4 starts here.\n");

    set_time.tm_year   = 116;
    set_time.tm_mon    = 1;
    set_time.tm_mday   = 29;
    set_time.tm_hour   = 23;
    set_time.tm_min    = 59;
    set_time.tm_sec    = 59;

    ret = csi_rtc_set_time(rtc, &set_time);
    ASSERT_TRUE(ret == 0);
    mdelay(2000);

    ret = csi_rtc_get_time(rtc, &get_time);
    ASSERT_TRUE(ret == 0);

    check_rtc_time(get_time, 116, 2, 1, 0, 0);

    /**************************************************************
     * Case-5 Set time to 2016-13-1 0:0:0
     ************************************************************/
    //printf("Case5 starts here.\n");

    set_time.tm_year   = 116;
    set_time.tm_mon    = 12;
    set_time.tm_mday   = 1;
    set_time.tm_hour   = 0;
    set_time.tm_min    = 0;
    set_time.tm_sec    = 0;

    ret = csi_rtc_set_time(rtc, &set_time);
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_RTC_BASE | RTC_ERROR_TIME));

    /**************************************************************
     * Case-6 Set time to 1969-12-31 0:0:0
     ************************************************************/
    //printf("Case6 starts here.\n");

    set_time.tm_year   = 69;
    set_time.tm_mon    = 11;
    set_time.tm_mday   = 31;
    set_time.tm_hour   = 0;
    set_time.tm_min    = 0;
    set_time.tm_sec    = 0;

    ret = csi_rtc_set_time(rtc, &set_time);
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_RTC_BASE | RTC_ERROR_TIME));

    /**************************************************************
     * Case-7 Set time to 2017-12-32 0:0:0
     ************************************************************/
    //printf("Case7 starts here.\n");

    set_time.tm_year   = 117;
    set_time.tm_mon    = 11;
    set_time.tm_mday   = 32;
    set_time.tm_hour   = 0;
    set_time.tm_min    = 0;
    set_time.tm_sec    = 0;

    ret = csi_rtc_set_time(rtc, &set_time);
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_RTC_BASE | RTC_ERROR_TIME));

    /**************************************************************
     * Case-8 Set time to 2016-2-30 0:0:0
     ************************************************************/
    //printf("Case8 starts here.\n");

    set_time.tm_year   = 116;
    set_time.tm_mon    = 1;
    set_time.tm_mday   = 30;
    set_time.tm_hour   = 0;
    set_time.tm_min    = 0;
    set_time.tm_sec    = 0;

    ret = csi_rtc_set_time(rtc, &set_time);
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_RTC_BASE | RTC_ERROR_TIME));

    /**************************************************************
     * Case-9 Set time to 2100-2-29 0:0:0
     ************************************************************/
    //printf("Case9 starts here.\n");

    set_time.tm_year   = 200;
    set_time.tm_mon    = 1;
    set_time.tm_mday   = 29;
    set_time.tm_hour   = 0;
    set_time.tm_min    = 0;
    set_time.tm_sec    = 0;

    ret = csi_rtc_set_time(rtc, &set_time);
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_RTC_BASE | RTC_ERROR_TIME));

    /**************************************************************
     * Case-10 Set time to 2017-4-31 0:0:0
     ************************************************************/
    //printf("Case10 starts here.\n");

    set_time.tm_year   = 117;
    set_time.tm_mon    = 3;
    set_time.tm_mday   = 31;
    set_time.tm_hour   = 0;
    set_time.tm_min    = 0;
    set_time.tm_sec    = 0;

    ret = csi_rtc_set_time(rtc, &set_time);
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_RTC_BASE | RTC_ERROR_TIME));

    /**************************************************************
     * Case-11 Set time to 2017-4-30 24:0:0
     ************************************************************/
    //printf("Case11 starts here.\n");

    set_time.tm_year   = 117;
    set_time.tm_mon    = 3;
    set_time.tm_mday   = 30;
    set_time.tm_hour   = 24;
    set_time.tm_min    = 0;
    set_time.tm_sec    = 0;

    ret = csi_rtc_set_time(rtc, &set_time);
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_RTC_BASE | RTC_ERROR_TIME));

    /**************************************************************
     * Case-12 Set time to 2017-4-30 0:60:0
     ************************************************************/
    //printf("Case12 starts here.\n");

    set_time.tm_year   = 117;
    set_time.tm_mon    = 3;
    set_time.tm_mday   = 30;
    set_time.tm_hour   = 0;
    set_time.tm_min    = 60;
    set_time.tm_sec    = 0;

    ret = csi_rtc_set_time(rtc, &set_time);
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_RTC_BASE | RTC_ERROR_TIME));

    /**************************************************************
     * Case-13 Set time to 2017-4-30 0:0:60
     ************************************************************/
    //printf("Case13 starts here.\n");

    set_time.tm_year   = 117;
    set_time.tm_mon    = 3;
    set_time.tm_mday   = 30;
    set_time.tm_hour   = 0;
    set_time.tm_min    = 0;
    set_time.tm_sec    = 60;

    ret = csi_rtc_set_time(rtc, &set_time);
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_RTC_BASE | RTC_ERROR_TIME));

    /**************************************************************
     * Case-14 Reset the same time
     ************************************************************/
    //printf("Case14 starts here.\n");

    set_time.tm_year   = 117;
    set_time.tm_mon    = 3;
    set_time.tm_mday   = 30;
    set_time.tm_hour   = 0;
    set_time.tm_min    = 0;
    set_time.tm_sec    = 0;

    ret = csi_rtc_set_time(rtc, &set_time);
    ASSERT_TRUE(ret == 0);

    mdelay(10000);

    ret = csi_rtc_set_time(rtc, &set_time);
    ASSERT_TRUE(ret == 0);

    ret = csi_rtc_get_time(rtc, &get_time);
    ASSERT_TRUE(ret == 0);

    check_rtc_time(get_time, 117, 3, 30, 0, 0);

    /**************************************************************
     * Case-15 Reset different time
     ************************************************************/
    //printf("Case15 starts here.\n");

    set_time.tm_year   = 117;
    set_time.tm_mon    = 3;
    set_time.tm_mday   = 30;
    set_time.tm_hour   = 0;
    set_time.tm_min    = 0;
    set_time.tm_sec    = 0;

    ret = csi_rtc_set_time(rtc, &set_time);
    ASSERT_TRUE(ret == 0);

    mdelay(1000);

    set_time.tm_year   = 116;
    set_time.tm_mon    = 4;
    set_time.tm_mday   = 30;
    set_time.tm_hour   = 10;
    set_time.tm_min    = 30;
    set_time.tm_sec    = 40;

    ret = csi_rtc_set_time(rtc, &set_time);
    ASSERT_TRUE(ret == 0);

    ret = csi_rtc_get_time(rtc, &get_time);
    ASSERT_TRUE(ret == 0);

    check_rtc_time(get_time, 116, 4, 30, 10, 30);
#endif
    /**************************************************************
     * Case-16 Set timeout time 0s
     ************************************************************/
    //printf("Case16 starts here.\n");

    set_time.tm_year   = 117;
    set_time.tm_mon    = 4;
    set_time.tm_mday   = 3;
    set_time.tm_hour   = 14;
    set_time.tm_min    = 0;
    set_time.tm_sec    = 0;

    ret = csi_rtc_set_time(rtc, &set_time);
    ASSERT_TRUE(ret == 0);

    mdelay(1000);

    /*ret = csi_rtc_enable_alarm(rtc, 1);

    ret = csi_rtc_set_alarm(rtc, &set_time);
    check_ret(ret, casename,16);

    printf("The RTC should be rang at this time\n");*/

    /**************************************************************
     * Case-17 Set timeout time 200s
     ************************************************************/
    /*printf("Case17 starts here.\n");
    set_time.tm_min    = 3;
    set_time.tm_sec    = 20;

    printf("The RTC should be rang after 200 seconds.\n");
    ret = csi_rtc_enable_alarm(rtc, 1);

    ret = csi_rtc_set_alarm(rtc, &set_time);
    ASSERT_TRUE(ret == 0);

    mdelay(200000+5000);

    printf("The RTC should be rang at this time\n");
    ret = csi_rtc_stop(rtc);
    ASSERT_TRUE(ret == 0);*/

    /**************************************************************
     * Case-18 Restart rtc after timeout
     ************************************************************/
    //printf("Case18 starts here.\n");
    ret = csi_rtc_stop(rtc);
    ASSERT_TRUE(ret == 0);
    mdelay(1000);
    //rtc_status_t status;

    status = csi_rtc_get_status(rtc);
    ASSERT_TRUE(status.active == 0);

    ret = csi_rtc_start(rtc);

    set_time.tm_min    = 0;
    set_time.tm_sec    = 0;
    ret = csi_rtc_set_time(rtc, &set_time);

//    set_time.tm_sec    = 10;
    set_time.tm_min    = 1;

    ret = csi_rtc_enable_alarm(rtc, 1);

    ret = csi_rtc_set_alarm(rtc, &set_time);
    ASSERT_TRUE(ret == 0);
#if 1
    status = csi_rtc_get_status(rtc);
    ASSERT_TRUE(status.active == 1);

    printf("The RTC should be rang after 60 seconds.\n");
    mdelay(60000);

    printf("The RTC should be rang at this time\n");
    mdelay(3000);
    /*
        set_time.tm_sec    = 21;

        printf("Reset timeout\n");
        ret = csi_rtc_set_alarm(rtc, &set_time);
        ASSERT_TRUE(ret == 0);

        mdelay(11000);

        printf("The RTC should be rang at this time\n");
    */
    ret = csi_rtc_stop(rtc);
    ASSERT_TRUE(ret == 0);

    /**************************************************************
     * Case-19 Reset rtc before timeout
     ************************************************************/
    /*printf("Case19 starts here.\n");

    ret = csi_rtc_start(rtc);

    ret = csi_rtc_enable_alarm(rtc, 1);

    set_time.tm_sec    = 0;
    ret = csi_rtc_set_time(rtc, &set_time);
    check_ret(ret, casename,19);

    mdelay(1000);

    set_time.tm_sec    = 10;
    printf("Start alarm!\n");
    ret = csi_rtc_set_alarm(rtc, &set_time);
    check_ret(ret, casename,19);

    mdelay(5000);

    printf("Stop alarm! 5 seconds has passed after alarm started.\n");

    set_time.tm_sec    = 15;

    printf("Reset timeout\n");
    ret = csi_rtc_set_alarm(rtc, &set_time);
    check_ret(ret,19 , casename);

    mdelay(12000);

    printf("The RTC should be rang at this time\n");*/

    /**************************************************************
     * Case-20 Continous timeout
     ************************************************************/
    /*printf("Case20 starts here.\n");
    for(i=0;i<100;i++)
    {
        set_time.tm_sec    = 0;
        ret = csi_rtc_set_time(rtc, &set_time);

        mdelay(1000);

        set_time.tm_sec    = 10;
        ret = csi_rtc_set_alarm(rtc, &set_time);
        check_ret(ret, casename,20);

        mdelay(10000);

    }

    ret = csi_rtc_stop(rtc);*/
#endif

    //csi_rtc_stop(rtc);
    /*
    uint8_t n=1;
    while(1){
        mdelay(1000);
        printf("%d\n",n);
        n++;
    }
    */
    ret = csi_rtc_uninitialize(rtc);
    ASSERT_TRUE(ret == 0);

}

int test_rtc(void)
{
    dtest_suite_info_t test_suite_info = {
        "test_rtc", NULL, NULL, NULL, NULL
    };

    dtest_suite_t *test_suite = dtest_add_suite(&test_suite_info);

    dtest_case_info_t test_case_info_array[] = {
        {"rtc_if_test", rtc_if_test, 1},
        {"rtc_func_test", rtc_func_test, 1},
        {NULL, NULL, 0}
    };

    dtest_add_cases(test_suite, test_case_info_array);

    return 0;
}

int main(void)
{
    dtest_register_cb(test_rtc);
    dtest();

    return 0;
}
