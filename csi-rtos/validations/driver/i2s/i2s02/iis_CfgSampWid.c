/*
 * Copyright (C) 2017-2019 Alibaba Group Holding Limited
 */

/******************************************************************************
 * @file     iis_CfgSampWid.c
 * @brief    test the interface of csi_i2s_config_sample_width()
 * @version  V1.0
 * @date     14. Jun 2019
 ******************************************************************************/

#include "iis_test.h"

int8_t iis_CfgSampWid_test(void)
{
    i2s_handle_t iis_0;
    int32_t r_value;
    uint8_t i, tc_num;

    iis_0 = csi_i2s_initialize(0, NULL);
    ASSERT_TRUE(iis_0 != NULL);

    r_value = csi_i2s_power_control(iis_0, DRV_POWER_FULL);
    ASSERT_TRUE(r_value == 0);

    cfgsampwid_testcase tc[] = {
        {NULL, 16, 0},
        {iis_0, 16, 1},
        {iis_0, 0, 0},
        {iis_0, 3, 0},
        {iis_0, 8, 0},
        {iis_0, 24, 1},
        {iis_0, 32, 0},
        {iis_0, 33, 0},
        {iis_0, 0xffff, 0},
    };

    tc_num = sizeof(tc) / sizeof(cfgsampwid_testcase);

    for (i = 0; i < tc_num; i++) {
        r_value = csi_i2s_config_sample_width(tc[i].handle, tc[i].samplewidth);

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
