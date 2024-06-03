/*
 * Copyright (C) 2017-2019 Alibaba Group Holding Limited
 */

/******************************************************************************
 * @file     iic_power_cont_if.c
 * @brief    iic power cont if test
 * @version  V1.0
 * @date     14. Jun 2019
 ******************************************************************************/

#include "iic_test.h"

typedef struct iic_power_control_testcase {
    iic_handle_t handle;
    csi_power_stat_e state;
    uint8_t y_n;    //sucess on 1,failed on 0,not supported on 2
} iic_power_cont_tc;

static void iic_callback(int32_t idx, iic_event_e event)
{
    printf("this is NO.%d iic.\n", idx);
}

int8_t iic_power_cont_if_test(void)
{
    int32_t r_value;
    iic_handle_t iic_handle;

    iic_handle = csi_iic_initialize(TEST_IIC_INDEX, iic_callback);

    if (iic_handle == NULL) {
        return -1;
    }

    iic_power_cont_tc testcase[] = {
        {NULL, DRV_POWER_OFF, 0},
        {iic_handle, DRV_POWER_FULL, 1},
        {iic_handle, DRV_POWER_LOW, 1},
        {iic_handle, DRV_POWER_OFF, 1},
        {iic_handle, 10, 0},
    };

    uint8_t i;
    uint8_t testcase_num = sizeof(testcase) / sizeof(iic_power_cont_tc);

    for (i = 0; i < testcase_num; i++) {
        r_value = csi_iic_power_control(testcase[i].handle, testcase[i].state);

        switch (testcase[i].y_n) {
            case 0:
                ASSERT_TRUE(r_value != 0);
                break;

            case 1:
                ASSERT_TRUE(r_value == 0);
                break;

            default :
                printf("testcase error!\n");
                break;
        }
    }

    r_value = csi_iic_uninitialize(iic_handle);

    if (r_value != 0) {
        return -2;
    }

    printf("iic power control test finished.\n");
    return 0;
}
