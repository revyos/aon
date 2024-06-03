/*
 * Copyright (C) 2017-2019 Alibaba Group Holding Limited
 */

/******************************************************************************
 * @file     iic_capa_if.c
 * @brief    iic capa_if test
 * @version  V1.0
 * @date     14. Jun 2019
 ******************************************************************************/

#include "iic_test.h"

typedef struct iic_get_capa_testcase {
    int32_t idx;
    uint8_t y_n;	//sucess on 1,failed on 0
} iic_capa_tc;

void iic_capa_if_test(void)
{
    iic_capabilities_t capa_n, capa_ck802 = {1}, capa_ck803 = {0};
    iic_capa_tc testcase[] = {
        { -1, 0},
        {TEST_IIC_INDEX, 1},
        {10, 0},
        {0xffff, 0},
    };

    uint8_t i;
    uint8_t testcase_num = sizeof(testcase) / sizeof(iic_capa_tc);

    for (i = 0; i < testcase_num; i++) {
        capa_n = csi_iic_get_capabilities(testcase[i].idx);

        switch (testcase[i].y_n) {
            case 0:
                ASSERT_TRUE(capa_n.address_10_bit == 0);
                break;

            case 1:
                ASSERT_TRUE(capa_n.address_10_bit == capa_ck803.address_10_bit);
                break;

            default :
                printf("testcase error!\n");
                break;
        }
    }

    printf("get capa test finished!\n");
}
