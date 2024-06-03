/*
 * Copyright (C) 2017-2019 Alibaba Group Holding Limited
 */

/******************************************************************************
 * @file     iis_Init.c
 * @brief    iis_Init_test
 * @version  V1.0
 * @date     14. Jun 2019
 ******************************************************************************/

#include "iis_test.h"

static void callback(int32_t idx, i2s_event_e event)
{
    switch (event) {
        default:
            break;
    }
}

int8_t iis_Init_test(void)
{
    i2s_handle_t iis_0;
    uint8_t i, tc_num;
    init_testcase tc[] = {
        { -1, NULL, 0},
        {0, NULL, 1},
        {0xffff, NULL, 0},
        {0, callback, 1},
    };

    tc_num = sizeof(tc) / sizeof(init_testcase);

    for (i = 0; i < tc_num; i++) {
        iis_0 = csi_i2s_initialize(tc[i].idx, tc[i].cb_event);

//		printf("[Debug-info]--%d\n",i);
        switch (tc[i].y_n) {
            case 0:
                ASSERT_TRUE(iis_0 == NULL);
                break;

            case 1:
                ASSERT_TRUE(iis_0 != NULL);
                break;

            default:
                break;
        }

        csi_i2s_uninitialize(iis_0);
    }

    return 0;
//	printf("init test over.\n");
}
