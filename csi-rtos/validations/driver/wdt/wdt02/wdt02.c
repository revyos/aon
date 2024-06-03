/*
 * Copyright (C) 2017-2019 Alibaba Group Holding Limited
 */

/******************************************************************************
 * @file     wdt02.c
 * @brief    wdt driver test
 * @version  V1.0
 * @date     14. Jun 2019
 ******************************************************************************/

#include "wdt_test.h"
#include <stdio.h>
#include <drv/intc.h>
#include "dtest.h"

void wdt_event_cb_func(int32_t idx, wdt_event_e event)
{
    if (event == WDT_EVENT_TIMEOUT) ;

    //printf("WDT%d event entered!\n", idx);
}

void wdt_if_test()
{
    //initialize
    wdt_handle_t wdt = csi_wdt_initialize(10, wdt_event_cb_func);
    ASSERT_TRUE(wdt == NULL);

    wdt = csi_wdt_initialize(0, NULL);
    ASSERT_TRUE(wdt != NULL);

    int32_t ret;
    //uninitialize
    ret = csi_wdt_uninitialize(NULL);
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_WDT_BASE | DRV_ERROR_PARAMETER));

    //set_timeout
    ret = csi_wdt_set_timeout(NULL, 100000);
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_WDT_BASE | DRV_ERROR_PARAMETER));

    //start
    ret = csi_wdt_start(NULL);
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_WDT_BASE | DRV_ERROR_PARAMETER));

    //stop
    ret = csi_wdt_stop(NULL);
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_WDT_BASE | DRV_ERROR_PARAMETER));

    //restart
    ret = csi_wdt_restart(NULL);
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_WDT_BASE | DRV_ERROR_PARAMETER));

    uint32_t value;
    //read_current_value
    ret = csi_wdt_read_current_value(NULL, &value);
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_WDT_BASE | DRV_ERROR_PARAMETER));

    ret = csi_wdt_read_current_value(wdt, NULL);
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_WDT_BASE | DRV_ERROR_PARAMETER));

    //power control
    ret = csi_wdt_power_control(NULL, DRV_POWER_OFF);
    //ASSERT_TRUE(ret == (CSI_DRV_ERRNO_WDT_BASE |DRV_ERROR_UNSUPPORTED));
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_WDT_BASE | DRV_ERROR_PARAMETER));
    ret = csi_wdt_power_control(wdt, 10);
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_WDT_BASE | DRV_ERROR_UNSUPPORTED));

    csi_wdt_uninitialize(wdt);
}

void wdt_func_test()
{
    uint32_t ret;
    wdt_handle_t wdt;

    /*******  Test get instance  *********/

    wdt = csi_wdt_initialize(WDT_INDEX, wdt_event_cb_func);

    if (wdt == NULL) {
        printf("Fail to init WDT, this test failed cannot be continued!\n");
        return;
    }

    /*******************************************
     *  Case-0 Use wdt
     *************************************************/
#if 0
    //uint32_t base,irq;
    //target_get_wdt(0, &base, &irq);
    //csi_intc_disable_irq(irq);
    //csi_intc_disable_irq(32);
    printf("Case-0 starts here.\n");
    ret = csi_wdt_set_timeout(wdt, WDT_TEST_DURATION);
    ASSERT_TRUE(ret == 0);

    printf("Watchdog start and the board will restart after %d seconds.\n", WDT_TEST_DURATION / 1000);
    ret = csi_wdt_start(wdt);
    ASSERT_TRUE(ret == 0);

    uint8_t i = 0;

    while (1) {
        i++;
        mdelay(1000);
        printf("%d\n", i);
    }

    //mdelay(WDT_TEST_DURATION); //silan's wdt need
    mdelay(WDT_TEST_DURATION - 1000);
    printf("After 1s , board will reset\n");
    mdelay(11000);
    printf("You shall not see this information\n");
#endif

    /************************************************
     * Case-1 Open the same wdt
     **************************************************/

    /*printf("Case1 starts here.\n");
    wdt1 = csi_wdt_initialize(0,wdt_event_cb);
    handle_conf(wdt1, casename, 0 , 1);*/

    /************************************************
     * Case-2 Open nonexisted wdt
     **************************************************/
    /*printf("Case2 starts here.\n");
    wdt1 = csi_wdt_initialize(1,wdt_event_cb);
    handle_conf(wdt1, casename, 0 , 2);*/

#if 1
    /************************************************
     * Case-3 Change timeout settings
     **************************************************/
    /*
    printf("Case3 starts here.\n");
    ret = csi_wdt_set_timeout(wdt, WDT_TEST_DURATION);
    printf("Watchdog start and the board will restart after %d seconds.\n", WDT_TEST_DURATION/1000);
    csi_wdt_start(wdt);

    mdelay(WDT_TEST_DURATION -1000);
    printf("Change wdt timeout settings to %d seconds\n", WDT_TEST_DURATION *2/1000);
    ret = csi_wdt_set_timeout(wdt, WDT_TEST_DURATION);
    ASSERT_TRUE(ret == 0);
    mdelay(WDT_TEST_DURATION *2  -1000);
    printf("After 1s , board will reset\n");    //must printf this

    mdelay(2000);
    printf("You shall not see this information\n");
    */

    //only sc5654a use
    printf("Case3 starts here.\n");
    ret = csi_wdt_set_timeout(wdt, WDT_TEST_DURATION / 2);
    printf("Watchdog start and the board will restart after %d seconds.\n", WDT_TEST_DURATION / 1000);
    csi_wdt_start(wdt);

    mdelay(WDT_TEST_DURATION / 2 - 1000);
    printf("Change wdt timeout settings to %d seconds\n", WDT_TEST_DURATION * 2 / 1000);
    ret = csi_wdt_set_timeout(wdt, WDT_TEST_DURATION);
    ASSERT_TRUE(ret == 0);
    mdelay(WDT_TEST_DURATION  - 1000);
    printf("After 1s , board will reset\n");	//must printf this

    mdelay(12000);
    printf("You shall not see this information\n");

#endif
    /************************************************
     * Case-4 Timeout is 0
     **************************************************/
    /*printf("Case4 starts here.\n");
    ret = csi_wdt_set_timeout(wdt, 0);
    check_ret(ret, casename,4);

    printf("Start wdt\n");
    ret = csi_wdt_start(wdt);
    check_ret(ret, casename,4);*/

    /************************************************
     * Case-5 Restart watchdog
     **************************************************/
#if 0
    printf("Case5 starts here.\n");
    ret = csi_wdt_set_timeout(wdt, WDT_TEST_DURATION);
    ASSERT_TRUE(ret == 0);

    ret = csi_wdt_start(wdt);
    ASSERT_TRUE(ret == 0);

    mdelay(WDT_TEST_DURATION - 1000);
    //k_delay_ms(WDT_TEST_DURATION/2);

    ret = csi_wdt_restart(wdt);
    ASSERT_TRUE(ret == 0);

    printf("Watchdog restarted, the board should not restart after 1 seconds\n");

    mdelay(5000);

    printf("You should see this message\n");

    ret = csi_wdt_stop(wdt);
    ASSERT_TRUE(ret == 0);

    mdelay(20000);
#endif

#if 0
    ret = csi_wdt_set_timeout(wdt, WDT_TEST_DURATION);
    ASSERT_TRUE(ret == 0);

    ret = csi_wdt_start(wdt);
    ASSERT_TRUE(ret == 0);

    mdelay(1000);
    uint32_t value1, value2;
    ret = csi_wdt_read_current_value(wdt, &value1);
    ASSERT_TRUE(ret == 0);

    mdelay(1000);
    ret = csi_wdt_read_current_value(wdt, &value2);
    ASSERT_TRUE(ret == 0);
    ASSERT_TRUE(value1 != value2);

    mdelay(1000);
    ret = csi_wdt_stop(wdt);
    ASSERT_TRUE(ret == 0);

    ret = csi_wdt_read_current_value(wdt, &value1);
    ASSERT_TRUE(ret == 0);

    mdelay(1000);
    ret = csi_wdt_read_current_value(wdt, &value2);
    ASSERT_TRUE(ret == 0);
    ASSERT_TRUE(value1 == value2);

#endif

    ret = csi_wdt_stop(wdt);
    ASSERT_TRUE(ret == 0);

    csi_wdt_uninitialize(wdt);
}

int test_wdt(void)
{
    dtest_suite_info_t test_suite_info = {
        "test_wdt", NULL, NULL, NULL, NULL
    };

    dtest_suite_t *test_suite = dtest_add_suite(&test_suite_info);

    dtest_case_info_t test_case_info_array[] = {
        {"wdt_if_test", wdt_if_test, 1},
        {"wdt_func_test", wdt_func_test, 1},
        {NULL, NULL, 0}
    };

    dtest_add_cases(test_suite, test_case_info_array);

    return 0;
}

int main(void)
{
    dtest_register_cb(test_wdt);
    dtest();

    return 0;
}
