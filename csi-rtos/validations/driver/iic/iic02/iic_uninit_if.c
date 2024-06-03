/*
 * Copyright (C) 2017-2019 Alibaba Group Holding Limited
 */

/******************************************************************************
 * @file     iic_uninit_if.c
 * @brief    iic ununit if test
 * @version  V1.0
 * @date     14. Jun 2019
 ******************************************************************************/

#include "iic_test.h"

typedef struct iic_uninit_testcase {
    iic_handle_t handle;
    uint8_t y_n;    //sucess on 1,failed on 0
} iic_uninit_tc;

static void iic_callback(int32_t idx, iic_event_e event)
{
    printf("this is NO.%d iic.\n", idx);
}

int8_t iic_uninit_if_test(void)
{
    int32_t r_value;
    iic_handle_t iic_handle;

    iic_handle = csi_iic_initialize(TEST_IIC_INDEX, iic_callback);
    ASSERT_TRUE(iic_handle != NULL);

    iic_uninit_tc testcase[] = {
        {NULL, 0},
        {iic_handle, 1},
    };

    uint8_t i;
    uint8_t testcase_num = sizeof(testcase) / sizeof(iic_uninit_tc);

    for (i = 0; i < testcase_num; i++) {
        r_value = csi_iic_uninitialize(testcase[i].handle);

        switch (testcase[i].y_n) {
            case 0:
                ASSERT_TRUE(r_value != 0);
                break;

            case 1:
                ASSERT_TRUE(r_value == 0);
                break;

            default :
                printf("iic uninit testcase error!\n");
                break;
        }
    }

    printf("iic uninit test finished.\n");
    return 0;
}
