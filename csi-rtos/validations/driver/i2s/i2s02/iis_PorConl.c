/*
 * Copyright (C) 2017-2019 Alibaba Group Holding Limited
 */

/******************************************************************************
 * @file     iis_PorConl.c
 * @brief    test the interface of csi_i2s_power_control()
 * @version  V1.0
 * @date     14. Jun 2019
 ******************************************************************************/

#include "iis_test.h"

int8_t iis_PorCont_test(void)
{
    i2s_handle_t iis_0;
    int32_t r_value;
    uint8_t i, tc_num;

    iis_0 = csi_i2s_initialize(0, NULL);
    ASSERT_TRUE(iis_0 != NULL);

    porcont_testcase tc[] = {
        {NULL, DRV_POWER_FULL, 0},
        {iis_0, DRV_POWER_FULL, 1},
        {iis_0, -1, 0x84},				//error code is 0x84
        {iis_0, 0xffff, 0x84},			//error code is 0x84
        {iis_0, DRV_POWER_LOW, 0x83},		//error code is 0x83
        {iis_0, DRV_POWER_OFF, 1},
    };

    tc_num = sizeof(tc) / sizeof(porcont_testcase);

    for (i = 0; i < tc_num; i++) {
        r_value = csi_i2s_power_control(tc[i].handle, tc[i].state);

        switch (tc[i].y_n) {
            case 0:
                ASSERT_TRUE(r_value < 0);
                break;

            case 1:
                ASSERT_TRUE(r_value == 0);
                break;

            case 0x83:
                ASSERT_TRUE(r_value == 0x83);
                break;

            case 0x84:
                ASSERT_TRUE(r_value == 0x84);
                break;

            default:
                ASSERT_TRUE(1 == -3);
                break;
        }
    }

    csi_i2s_uninitialize(iis_0);
    return 0;
}
