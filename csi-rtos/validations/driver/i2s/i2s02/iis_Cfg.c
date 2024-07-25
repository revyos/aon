/*
 * Copyright (C) 2017-2019 Alibaba Group Holding Limited
 */

/******************************************************************************
 * @file     iis_Cfg.c
 * @brief    test the interface of csi_i2s_config()
 * @version  V1.0
 * @date     14. Jun 2019
 ******************************************************************************/

#include "iis_test.h"

int8_t iis_Cfg_test(void)
{
    i2s_handle_t iis_0;
    int32_t r_value;

    iis_0 = csi_i2s_initialize(0, NULL);
    ASSERT_TRUE(iis_0 != NULL);

    r_value = csi_i2s_power_control(iis_0, DRV_POWER_FULL);
    ASSERT_TRUE(r_value == 0);

    r_value = csi_i2s_config(NULL, I2S_PROTOCOL_I2S, 16, I2S_MODE_TX_MASTER, 48000);
    ASSERT_TRUE(r_value < 0);

    r_value = csi_i2s_config(iis_0, I2S_PROTOCOL_I2S, 16, I2S_MODE_TX_MASTER, 48000);
    ASSERT_TRUE(r_value == 0);

    csi_i2s_power_control(iis_0, DRV_POWER_OFF);
    csi_i2s_uninitialize(iis_0);
    return 0;
}
