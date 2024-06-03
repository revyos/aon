/*
 * Copyright (C) 2017-2019 Alibaba Group Holding Limited
 */

/******************************************************************************
 * @file     iis_GetStat.c
 * @brief    test the interface of csi_i2s_get_status()
 * @version  V1.0
 * @date     14. Jun 2019
 ******************************************************************************/

#include "iis_test.h"
#include <stdlib.h>
#include <string.h>

int8_t iis_GetStat_test(void)
{
    i2s_status_t stat_iis_get, stat_iis_err = {0, 0, 0, 0, 0};
    int r_cmp;

    memset(&stat_iis_get, 0, sizeof(i2s_status_t));
    memset(&stat_iis_err, 0, sizeof(i2s_status_t));

    stat_iis_get = csi_i2s_get_status(NULL);
    r_cmp = memcmp(&stat_iis_get, &stat_iis_err, sizeof(i2s_status_t));
    ASSERT_TRUE(r_cmp == 0);

    return 0;
}
