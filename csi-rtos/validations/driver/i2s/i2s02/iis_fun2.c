/*
 * Copyright (C) 2017-2019 Alibaba Group Holding Limited
 */

/******************************************************************************
 * @file     iis_fun2.c
 * @brief    test the iis send audio data in inquiry mode
 * @version  V1.0
 * @date     14. Jun 2019
 ******************************************************************************/

#include "iis_test.h"
#include <stdlib.h>
#include <string.h>

int8_t iis_fun2_test(void)
{
    i2s_handle_t iis_0;
    int32_t r_value;
//    uint8_t tc_num;
    int r_cmp;
    const uint8_t send_data[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9}; //there need a true audio data
    i2s_status_t stat_iis_get, stat_iis_0 = {0, 0, 0, 0, 0}, stat_iis_busy = {1, 0, 0, 0, 0};

    memset(&stat_iis_get, 0, sizeof(i2s_status_t));

    iis_0 = csi_i2s_initialize(0, NULL);
    ASSERT_TRUE(iis_0 != NULL);

    r_value = csi_i2s_power_control(iis_0, DRV_POWER_FULL);
    ASSERT_TRUE(r_value == 0);

    /* get the iis's status*/
    stat_iis_get = csi_i2s_get_status(iis_0);
    r_cmp = memcmp(&stat_iis_get, &stat_iis_0, sizeof(i2s_status_t));
    ASSERT_TRUE(r_cmp == 0);

    /* config the iis */
    r_value = csi_i2s_config(iis_0, I2S_PROTOCOL_LSB_JUSTIFIED, 24, I2S_MODE_TX_MASTER, 44100);
    ASSERT_TRUE(r_value == 0);

    /* send data */
    r_value = csi_i2s_send(iis_0, send_data, sizeof(send_data));
    ASSERT_TRUE(r_value == 0);

    do {
        stat_iis_get = csi_i2s_get_status(iis_0);
        r_cmp = memcmp(&stat_iis_get, &stat_iis_busy, sizeof(i2s_status_t));
        ASSERT_TRUE(r_cmp == 0);
    } while (csi_i2s_get_status(iis_0).tx_busy == 1); //inquiry if send complete


    csi_i2s_power_control(iis_0, DRV_POWER_OFF);
    csi_i2s_uninitialize(iis_0);
    return 0;
}
