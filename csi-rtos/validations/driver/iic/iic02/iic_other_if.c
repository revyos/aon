/*
 * Copyright (C) 2017-2019 Alibaba Group Holding Limited
 */

/******************************************************************************
 * @file     iic_other_if.c
 * @brief    iic other if test
 * @version  V1.0
 * @date     14. Jun 2019
 ******************************************************************************/

#include "iic_test.h"

typedef struct iic_other_interface_testcase {
    iic_handle_t handle;
    uint8_t y_n;    //sucess on 1,failed on 0
} iic_other_tc;

static void iic_callback(int32_t idx, iic_event_e event)
{
    printf("this is NO.%d iic.\n", idx);
}

int8_t iic_other_if_test(void)
{
    int32_t r_value;
    iic_status_t iic_status;
    uint32_t r_num;
    iic_handle_t iic_handle;

    iic_handle = csi_iic_initialize(TEST_IIC_INDEX, iic_callback);

    if (iic_handle == NULL) {
        return -1;
    }

    iic_other_tc testcase[] = {
        {NULL, 0},
        {iic_handle, 1},
    };

    uint8_t i;
    uint8_t testcase_num = sizeof(testcase) / sizeof(iic_other_tc);

    for (i = 0; i < testcase_num; i++) {
        r_value = csi_iic_abort_transfer(testcase[i].handle);

        switch (testcase[i].y_n) {
            case 0:
                ASSERT_TRUE(r_value != 0);
                break;

            case 1:
                ASSERT_TRUE(r_value == 0);
                break;

            default :
                printf("1.testcase error!\n");
                break;
        }
    }

    for (i = 0; i < testcase_num; i++) {
        iic_status = csi_iic_get_status(testcase[i].handle);

        switch (testcase[i].y_n) {
            case 0:
                ASSERT_TRUE((iic_status.busy == 0) && (iic_status.mode == 0) && (iic_status.direction == 0) && (iic_status.general_call == 0) && (iic_status.arbitration_lost == 0) && (iic_status.bus_error == 0));
                break;

            case 1:
                break;

            default :
                printf("2.testcase error!\n");
                break;
        }
    }

    for (i = 0; i < testcase_num; i++) {
        r_num = csi_iic_get_data_count(testcase[i].handle);

        switch (testcase[i].y_n) {
            case 0:
                ASSERT_TRUE(r_num == 0);
                break;

            case 1:
                break;

            default :
                printf("3.testcase error!\n");
                break;
        }
    }

    r_value = csi_iic_uninitialize(iic_handle);

    if (r_value != 0) {
        return -2;
    }

    printf("iic other interface test finished.\n");
    return 0;
}
