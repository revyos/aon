/*
 * Copyright (C) 2017-2019 Alibaba Group Holding Limited
 */

/******************************************************************************
 * @file     iis_DmaRecv.c
 * @brief    test the interface parameters of 'csi_i2s_dma_send' function
 * @version  V1.0
 * @date     14. Jun 2019
 ******************************************************************************/

#include "iis_test.h"

int8_t iis_DmaRecv_test(void)
{
    i2s_handle_t iis_0;
    int32_t r_value;
    uint8_t i, tc_num;
    uint32_t recv_data[100];

    iis_0 = csi_i2s_initialize(0, NULL);
    ASSERT_TRUE(iis_0 != NULL);

    r_value = csi_i2s_power_control(iis_0, DRV_POWER_FULL);
    ASSERT_TRUE(r_value == 0);

    recv_testcase tc[] = {
        {NULL, recv_data, 16, 0},
        {iis_0, recv_data, 16, 1},
        {iis_0, NULL, 4, 0},
        {iis_0, recv_data, 0, 0}
    };

    tc_num = sizeof(tc) / sizeof(recv_testcase);

    for (i = 0; i < tc_num; i++) {
        r_value = csi_i2s_dma_receive(tc[i].handle, tc[i].data, tc[i].data_size);

        switch (tc[i].y_n) {
            case 0:
                ASSERT_TRUE(r_value < 0);
                break;

            case 1:
                ASSERT_TRUE(r_value == 0);
                break;

            default:
                ASSERT_TRUE(1 == -3);
                break;
        }
    }

    csi_i2s_power_control(iis_0, DRV_POWER_OFF);
    csi_i2s_uninitialize(iis_0);
    return 0;
}
