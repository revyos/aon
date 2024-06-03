/*
 * Copyright (C) 2017-2019 Alibaba Group Holding Limited
 */

/******************************************************************************
 * @file     iis_Uninit.c
 * @brief    iis_Uninit_test
 * @version  V1.0
 * @date     14. Jun 2019
 ******************************************************************************/

#include "iis_test.h"

int8_t iis_Uninit_test(void)
{
    i2s_handle_t iis_0;
    int32_t r_value;

    iis_0 = csi_i2s_initialize(0, NULL);
    ASSERT_TRUE(iis_0 != NULL);

    r_value = csi_i2s_uninitialize(NULL);
    ASSERT_TRUE(r_value != 0);

    r_value = csi_i2s_uninitialize(iis_0);
    ASSERT_TRUE(r_value == 0);

    printf("uninit test over.\n");
    return 0;
}
