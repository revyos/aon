/*
 * Copyright (C) 2017-2019 Alibaba Group Holding Limited
 */

/******************************************************************************
 * @file     iic_config_if.c
 * @brief    iic config if test
 * @version  V1.0
 * @date     14. Jun 2019
 ******************************************************************************/

#include "iic_test.h"

typedef struct iic_config_mode_testcase {
    iic_handle_t handle;
    iic_mode_e mode;
    uint8_t y_n;    //sucess on 1,failed on 0
} iic_mode_tc;

typedef struct iic_config_speed_testcase {
    iic_handle_t handle;
    iic_speed_e speed;
    uint8_t y_n;    //sucess on 1,failed on 0
} iic_speed_tc;

typedef struct iic_config_addr_mode_testcase {
    iic_handle_t handle;
    iic_address_mode_e addr_mode;
    uint8_t y_n;    //sucess on 1,failed on 0
} iic_addr_mode_tc;

typedef struct iic_config_slave_addr_testcase {
    iic_handle_t handle;
    int32_t slave_addr;
    uint8_t y_n;    //sucess on 1,failed on 0
} iic_slave_addr_tc;

static void iic_callback(int32_t idx, iic_event_e event)
{
    printf("this is NO.%d iic.\n", idx);
}

int8_t iic_config_if_test(void)
{
    int32_t r_value;
    iic_handle_t iic_handle;

    csi_pin_set_mux(TEST_IIC_SDA_PIN, TEST_SDA_PIN_FUNCTION);
    csi_pin_set_mux(TEST_IIC_SCL_PIN, TEST_SCL_PIN_FUNCTION);


    iic_handle = csi_iic_initialize(TEST_IIC_INDEX, iic_callback);

    if (iic_handle == NULL) {
        return -1;
    }

    iic_mode_tc testcase1[] = {
        {NULL, IIC_MODE_MASTER, 0},
        {iic_handle, IIC_MODE_MASTER, 1},
        {iic_handle, IIC_MODE_SLAVE, 0},
        {iic_handle, 0xffff, 0}
    };

    iic_speed_tc testcase2[] = {
        {NULL, IIC_BUS_SPEED_STANDARD, 0},
        {iic_handle, IIC_BUS_SPEED_STANDARD, 1},
        {iic_handle, IIC_BUS_SPEED_FAST, 1},
        {iic_handle, IIC_BUS_SPEED_FAST_PLUS, 0},
        {iic_handle, IIC_BUS_SPEED_HIGH, 1},
        {iic_handle, 0xffff, 0},
        {iic_handle, 10, 0},
    };

    iic_addr_mode_tc testcase3[] = {
        {NULL, IIC_ADDRESS_7BIT, 0},
        {iic_handle, IIC_ADDRESS_7BIT, 1},
        {iic_handle, IIC_ADDRESS_10BIT, 1},
        {iic_handle, 0xffff, 0},
        {iic_handle, 10, 0},
    };

    iic_slave_addr_tc testcase4[] = {
        {NULL, 0x38, 0},
        {iic_handle, 0x38, 1},
        {iic_handle, 0, 1},
        {iic_handle, 0xffff, 1},
    };

    uint8_t i, testcase1_num, testcase2_num, testcase3_num, testcase4_num;

    testcase1_num = sizeof(testcase1) / sizeof(iic_mode_tc);
    testcase2_num = sizeof(testcase2) / sizeof(iic_speed_tc);
    testcase3_num = sizeof(testcase3) / sizeof(iic_addr_mode_tc);
    testcase4_num = sizeof(testcase4) / sizeof(iic_slave_addr_tc);

    for (i = 0; i < testcase1_num; i++) {
        r_value = csi_iic_config_mode(testcase1[i].handle, testcase1[i].mode);

        switch (testcase1[i].y_n) {
            case 0:
                printf("++++%d\n", i);
                ASSERT_TRUE(r_value < 0);
                break;

            case 1:
//				printf("++++%d\n",i);
                ASSERT_TRUE(r_value == 0);
                break;

            default :
                printf("testcase error!\n");
                break;
        }
    }

    for (i = 0; i < testcase2_num; i++) {
        r_value = csi_iic_config_speed(testcase2[i].handle, testcase2[i].speed);

        switch (testcase2[i].y_n) {
            case 0:
//				printf("++++%d\n",i);
                ASSERT_TRUE(r_value < 0);
                break;

            case 1:
                //printf("++++%d\n",i);
                ASSERT_TRUE(r_value == 0);
                break;

            default :
//				printf("++++%d\n",i);
                printf("testcase error!\n");
                break;
        }
    }

    for (i = 0; i < testcase3_num; i++) {
        r_value = csi_iic_config_addr_mode(testcase3[i].handle, testcase3[i].addr_mode);

        switch (testcase3[i].y_n) {
            case 0:
                ASSERT_TRUE(r_value < 0);
                break;

            case 1:
                ASSERT_TRUE(r_value == 0);
                break;

            default :
                printf("testcase error!\n");
                break;
        }
    }

    for (i = 0; i < testcase4_num; i++) {
        r_value = csi_iic_config_slave_addr(testcase4[i].handle, testcase4[i].slave_addr);

        switch (testcase4[i].y_n) {
            case 0:
                ASSERT_TRUE(r_value < 0);
                break;

            case 1:
                ASSERT_TRUE(r_value == 0);
                break;

            default :
                printf("testcase error!\n");
                break;
        }
    }

    r_value = csi_iic_uninitialize(iic_handle);

    if (r_value != 0) {
        return -2;
    }

    printf("iic config interface test finished.\n");
    return 0;
}
