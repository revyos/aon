/*
 * Copyright (C) 2017-2019 Alibaba Group Holding Limited
 */

/******************************************************************************
 * @file     trng02.c
 * @brief    trng driver test
 * @version  V1.0
 * @date     14. Jun 2019
 ******************************************************************************/

#include "trng_test.h"
#include <stdio.h>
#include "dtest.h"

void trng_cb(int32_t idx, trng_event_e event)
{
    if (event == TRNG_EVENT_DATA_GENERATE_COMPLETE) {
        printf("Trng%d CB entered\n", idx);
    }
}

void trng_if_test()
{
    //get capabilities
    trng_capabilities_t capa;
    capa = csi_trng_get_capabilities(2);
    ASSERT_TRUE(capa.lowper_mode == 0);

    //initialize
    trng_handle_t trng = csi_trng_initialize(1, trng_cb);
    ASSERT_TRUE(trng == NULL);

    trng = csi_trng_initialize(0, NULL);
    ASSERT_TRUE(trng != NULL);

    int32_t ret;
    //uninitialize
    ret = csi_trng_uninitialize(NULL);
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_TRNG_BASE | DRV_ERROR_PARAMETER));

    //power_control
    ret = csi_trng_power_control(NULL, DRV_POWER_FULL);
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_TRNG_BASE | DRV_ERROR_UNSUPPORTED));
    ret = csi_trng_power_control(trng, 5);
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_TRNG_BASE | DRV_ERROR_UNSUPPORTED));

    //get_status
    trng_status_t stat = csi_trng_get_status(NULL);
    ASSERT_TRUE(stat.busy == 0);

    char data[128];
    //get_data
    ret = csi_trng_get_data(NULL, data, 128);
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_TRNG_BASE | DRV_ERROR_PARAMETER));

    ret = csi_trng_get_data(trng, NULL, 128);
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_TRNG_BASE | DRV_ERROR_PARAMETER));

    ret = csi_trng_get_data(trng, data, 0);
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_TRNG_BASE | DRV_ERROR_PARAMETER));

    ret = csi_trng_uninitialize(trng);
}

void trng_func_test()
{
    int i = 0, count;
    uint32_t ret;
    char data[100];
    trng_handle_t trng;

    /*******  Test get instance  *********/
    trng = csi_trng_initialize(0, trng_cb);
    ASSERT_TRUE(trng != NULL);

    /*********** Get 100 bytes data   **********************/

    ret = csi_trng_get_data(trng, data, 100);
    ASSERT_TRUE(ret == 0);
    printf("Random get data 100byte:");

    for (i = 0; i < 100; i++) {
        printf("%x", data[i]);
    }

    printf("\n");

    /*********** Get 1 byte data for 1000 times ***************/
    printf("get 1 byte data for 1000 times:");

    for (i = 0; i < 1000; i++) {
        ret = csi_trng_get_data(trng, &data[0], 1);
        ASSERT_TRUE(ret == 0);
        printf("%x", data[0]);
    }

    printf("\n");

    csi_trng_uninitialize(trng);

//    printf("Test finished\n");
}

int test_trng(void)
{
    dtest_suite_info_t test_suite_info = {
        "test_trng", NULL, NULL, NULL, NULL
    };

    dtest_suite_t *test_suite = dtest_add_suite(&test_suite_info);

    dtest_case_info_t test_case_info_array[] = {
        {"trng_if_test", trng_if_test, 1},
        {"trng_func_test", trng_func_test, 1},
        {NULL, NULL, 0}
    };

    dtest_add_cases(test_suite, test_case_info_array);

    return 0;
}

int main(void)
{
    dtest_register_cb(test_trng);
    dtest();

    return 0;
}
