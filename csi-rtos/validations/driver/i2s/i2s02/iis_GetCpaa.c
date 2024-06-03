/*
 * Copyright (C) 2017-2019 Alibaba Group Holding Limited
 */

/******************************************************************************
 * @file     iis_GetCpaa.c
 * @brief    iis_GetCapa_test
 * @version  V1.0
 * @date     14. Jun 2019
 ******************************************************************************/

#include "iis_test.h"
#include <stdlib.h>
#include <string.h>

int8_t iis_GetCapa_test(void)
{
    i2s_capabilities_t capa_iis_get, capa_iis_0 = {1, 0, 0, 1, 1, 0, 0, 0, 1}, capa_iis_err = {0, 0, 0, 0, 0, 0, 0, 0, 0};
    int r_cmp;

    /*
     *	must first initialize to 0,then call memcmp() will return success
     */
    memset(&capa_iis_get, 0, sizeof(i2s_capabilities_t));
    memset(&capa_iis_err, 0, sizeof(i2s_capabilities_t));
    memset(&capa_iis_0, 0, sizeof(i2s_capabilities_t));
    capa_iis_0.asynchronous = 1;
    capa_iis_0.synchronous = 0;
    capa_iis_0.protocol_user = 0;
    capa_iis_0.protocol_i2s = 1;
    capa_iis_0.protocol_justified = 1;
    capa_iis_0.mono_mode = 0;
    capa_iis_0.companding = 0;
    capa_iis_0.mclk_pin = 0;
    capa_iis_0.event_frame_error = 1;

    capa_iis_get = csi_i2s_get_capabilities(-1);
    r_cmp = memcmp(&capa_iis_get, &capa_iis_err, sizeof(i2s_capabilities_t));
    ASSERT_TRUE(r_cmp == 0);

    capa_iis_get = csi_i2s_get_capabilities(8);
    r_cmp = memcmp(&capa_iis_get, &capa_iis_err, sizeof(i2s_capabilities_t));
    ASSERT_TRUE(r_cmp == 0);

    capa_iis_get = csi_i2s_get_capabilities(0xffff);
    r_cmp = memcmp(&capa_iis_get, &capa_iis_err, sizeof(i2s_capabilities_t));
    ASSERT_TRUE(r_cmp == 0);

    capa_iis_get = csi_i2s_get_capabilities(0);
    r_cmp = memcmp(&capa_iis_get, &capa_iis_0, sizeof(i2s_capabilities_t));
    ASSERT_TRUE(r_cmp == 0);

    return 0;
}
