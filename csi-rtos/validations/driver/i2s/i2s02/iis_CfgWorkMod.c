/*
 * Copyright (C) 2017-2019 Alibaba Group Holding Limited
 */

/******************************************************************************
 * @file     iis_CfgWorkMod.c
 * @brief    test the interface of csi_i2s_config_work_mode()
 * @version  V1.0
 * @date     14. Jun 2019
 ******************************************************************************/

#include "iis_test.h"

int8_t iis_CfgWorkMod_test(void)
{
    i2s_handle_t iis_0;
    int32_t r_value;
    uint8_t i, tc_num;

    iis_0 = csi_i2s_initialize(0, NULL);
    ASSERT_TRUE(iis_0 != NULL);

    r_value = csi_i2s_power_control(iis_0, DRV_POWER_FULL);
    ASSERT_TRUE(r_value == 0);

    cfgworkmod_testcase tc[] = {
        {NULL, I2S_MODE_TX_MASTER, 0},
        {iis_0, I2S_MODE_TX_MASTER, 1},
        {iis_0, -1, 0},
        {iis_0, 0xffff, 0},
        {iis_0, I2S_MODE_TX_SLAVE, 1},
        {iis_0, I2S_MODE_RX_MASTER, 1},
        {iis_0, I2S_MODE_RX_SLAVE, 1},
    };

    tc_num = sizeof(tc) / sizeof(cfgworkmod_testcase);

    for (i = 0; i < tc_num; i++) {
        r_value = csi_i2s_config_work_mode(tc[i].handle, tc[i].mode);

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
