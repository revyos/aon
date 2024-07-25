/*
 * Copyright (C) 2017-2019 Alibaba Group Holding Limited
 */

/******************************************************************************
 * @file     iis_fun1.c
 * @brief    test the iis send audio data in interrupted(callback) mode
 * @version  V1.0
 * @date     14. Jun 2019
 ******************************************************************************/

#include "iis_test.h"
#include <stdlib.h>
#include <string.h>

static uint8_t lock;
static void callback(int32_t idx, i2s_event_e event)
{
    switch (event) {
        case I2S_EVENT_SEND_COMPLETE:
            lock = 0;
            break;
        default:
            break;
    }

}

int8_t iis_fun1_test(void)
{
    i2s_handle_t iis_0;
    int32_t r_value;
//    uint8_t tc_num;
    int r_cmp;
    const uint8_t send_data[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0}; //there need a true audio data
    i2s_status_t stat_iis_get, stat_iis_0 = {0, 0, 0, 0, 0}, stat_iis_busy = {1, 0, 0, 0, 0};

    memset(&stat_iis_get, 0, sizeof(i2s_status_t));

    iis_0 = csi_i2s_initialize(0, callback);
    ASSERT_TRUE(iis_0 != NULL);

    r_value = csi_i2s_power_control(iis_0, DRV_POWER_FULL);
    ASSERT_TRUE(r_value == 0);

    /* get the iis's status*/
    stat_iis_get = csi_i2s_get_status(iis_0);
    r_cmp = memcmp(&stat_iis_get, &stat_iis_0, sizeof(i2s_status_t));
    ASSERT_TRUE(r_cmp == 0);

    /* config the iis */
    r_value = csi_i2s_config_protocol(iis_0, I2S_PROTOCOL_I2S);
    ASSERT_TRUE(r_value == 0);
    r_value = csi_i2s_config_sample_width(iis_0, 16);
    ASSERT_TRUE(r_value == 0);
    r_value = csi_i2s_config_work_mode(iis_0, I2S_MODE_TX_MASTER);
    ASSERT_TRUE(r_value == 0);
    r_value = csi_i2s_config_sample_rate(iis_0, 48000);
    ASSERT_TRUE(r_value == 0);

    /* send data */
    lock = 1;
    r_value = csi_i2s_send(iis_0, send_data, sizeof(send_data));
    ASSERT_TRUE(r_value == 0);

    stat_iis_get = csi_i2s_get_status(iis_0); //get the iis's status
    r_cmp = memcmp(&stat_iis_get, &stat_iis_busy, sizeof(i2s_status_t));
    ASSERT_TRUE(r_cmp == 0);

    while (lock) { //wait for send complete
        ;
    }

    stat_iis_get = csi_i2s_get_status(iis_0); //get the iis's status
    r_cmp = memcmp(&stat_iis_get, &stat_iis_0, sizeof(i2s_status_t));
    ASSERT_TRUE(r_cmp == 0);

    csi_i2s_power_control(iis_0, DRV_POWER_OFF);
    csi_i2s_uninitialize(iis_0);
    return 0;
}
