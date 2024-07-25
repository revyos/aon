/*
 * Copyright (C) 2017-2019 Alibaba Group Holding Limited
 */

/******************************************************************************
 * @file     iic_init_if.c
 * @brief    iic init if test
 * @version  V1.0
 * @date     14. Jun 2019
 ******************************************************************************/

#include "iic_test.h"

typedef struct iic_init_testcase {
    int32_t idx;
    iic_event_cb_t cb_event;
    uint8_t y_n;    //sucess on 1,failed on 0
} iic_init_tc;

static void iic_callback(int32_t idx, iic_event_e event)
{
    printf("this is NO.%d iic.\n", idx);
}

void iic_init_if_test(void)
{
    iic_handle_t iic_handle;
    iic_init_tc testcase[] = {
        { -1, iic_callback, 0},
        {TEST_IIC_INDEX, NULL, 1},
        {TEST_IIC_INDEX, iic_callback, 1},
        {0xffff, iic_callback, 0},
    };

    uint8_t i;
    uint8_t tsetcase_num = sizeof(testcase) / sizeof(iic_init_tc);

    for (i = 0; i < tsetcase_num; i++) {
        iic_handle = csi_iic_initialize(testcase[i].idx, testcase[i].cb_event);

        switch (testcase[i].y_n) {
            case 0:
                ASSERT_TRUE(iic_handle == NULL);
                break;

            case 1:
                ASSERT_TRUE(iic_handle != NULL);
                break;

            default :
                printf("init testcse error!\n");
                break;
        }
    }

    csi_iic_uninitialize(iic_handle);
    printf("iic init test finished.\n");
}
