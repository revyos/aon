/*
 * Copyright (C) 2017-2019 Alibaba Group Holding Limited
 */

/******************************************************************************
 * @file     iic_slave_receive_if.c
 * @brief    iic slave receive_if test 
 * @version  V1.0
 * @date     14. Jun 2019
 ******************************************************************************/

#include "iic_test.h"

typedef struct iic_slave_receive_testcase {
    iic_handle_t handle;
    void *data;
    uint32_t num;
    uint8_t y_n;    //sucess on 1,failed on 0
} iic_slave_receive_tc;

static void iic_callback(int32_t idx, iic_event_e event)
{
    printf("this is NO.%d iic.\n", idx);
}

int8_t iic_slave_receive_if_test(void)
{
    int32_t r_value;
    iic_handle_t iic_handle;

    iic_handle = csi_iic_initialize(TEST_IIC_INDEX, iic_callback);

    if (iic_handle == NULL) {
        return -1;
    }

    uint8_t receive_data[10] = {0};
    iic_slave_receive_tc testcase[] = {
        {NULL, receive_data, 10, 0},
        {iic_handle, receive_data, 10, 1},
        {iic_handle, NULL, 10, 0},
        {iic_handle, receive_data, 5, 1},
    };

    uint8_t i;
    uint8_t tsetcase_num = sizeof(testcase) / sizeof(iic_slave_receive_tc);

    for (i = 0; i < tsetcase_num; i++) {
        r_value = csi_iic_slave_receive(testcase[i].handle, testcase[i].data, testcase[i].num);

        switch (testcase[i].y_n) {
            case 0 :
                ASSERT_TRUE(r_value != 0);
                break;

            case 1 :
                ASSERT_TRUE(r_value == (CSI_DRV_ERRNO_IIC_BASE | DRV_ERROR_UNSUPPORTED)); //hobit3
//                ASSERT_TRUE(r_value == 0);
                break;

            default :
                printf("testcase error!\n");
                break;
        }
    }

    printf("iic slave receive finished.\n");
    r_value = csi_iic_uninitialize(iic_handle);

    if (r_value != 0) {
        return -2;
    }

    return 0;
}
