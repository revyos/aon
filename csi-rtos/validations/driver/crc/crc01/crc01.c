/*
 * Copyright (C) 2017-2019 Alibaba Group Holding Limited
 */

/******************************************************************************
 * @file     test_crc.c
 * @brief    the main function for the CRC driver
 * @version  V1.0
 * @date     20. July 2016
 ******************************************************************************/
#include <stdio.h>
#include "soc.h"
#include "drv/crc.h"
#include "dtest.h"
#include "test_driver_config.h"

typedef struct {
    crc_mode_e mode;
    crc_standard_crc_e standard;
    uint32_t *input;
    int32_t *expect_out;
} crc_test_t;
static crc_handle_t  *crc = NULL;

static void crc_get_num_cb(int32_t idx, crc_event_e event)
{
    printf("the crc calculate complete\r\n");
}

static uint32_t crc_input = 0x44332211;
static int32_t expect_out[] = {0xd1b1, 0xf5b1, 0xa4e, 0x2e4e, 0xf173, 0xdad, 0xdd33, 0x5f47, 0x59f3, 0x33, 0x41, 0xf9, 0xac, 0x77f29dd1, 0xb14257cc};

static crc_test_t crc_cases[] = {
#if CRC_MODBUS_16
    {CRC_MODE_CRC16, CRC_STANDARD_CRC_MODBUS, &crc_input, &expect_out[0]},
#endif
#if CRC_IBM_16
    {CRC_MODE_CRC16, CRC_STANDARD_CRC_IBM, &crc_input, &expect_out[1]},
#endif
#if CRC_MAXIM_16
    {CRC_MODE_CRC16, CRC_STANDARD_CRC_MAXIM, &crc_input, &expect_out[2]},
#endif
#if CRC_USB_16
    {CRC_MODE_CRC16, CRC_STANDARD_CRC_USB, &crc_input, &expect_out[3]},
#endif
#if CRC_CCITT_16
    {CRC_MODE_CRC16, CRC_STANDARD_CRC_CCITT, &crc_input, &expect_out[4]},
#endif
#if CRC_X25_16
    {CRC_MODE_CRC16, CRC_STANDARD_CRC_X25, &crc_input, &expect_out[5]},
#endif
#if CRC_XMODEM_16
    {CRC_MODE_CRC16, CRC_STANDARD_CRC_XMODEM, &crc_input, &expect_out[6]},
#endif
#if CRC_DNP_16
    {CRC_MODE_CRC16, CRC_STANDARD_CRC_DNP, &crc_input, &expect_out[7]},
#endif
#if CRC_CCITT_FALSE_16
    {CRC_MODE_CRC16, CRC_STANDARD_CRC_CCITT_FALSE, &crc_input, &expect_out[8]},
#endif
#if CRC_MAXIM_8
    {CRC_MODE_CRC8, CRC_STANDARD_CRC_MAXIM, &crc_input, &expect_out[9]},
#endif
#if CRC_ROHC_8
    {CRC_MODE_CRC8, CRC_STANDARD_CRC_ROHC, &crc_input, &expect_out[10]},
#endif
#if CRC_NONE_8
    {CRC_MODE_CRC8, CRC_STANDARD_CRC_NONE, &crc_input, &expect_out[11]},
#endif
#if CRC_ITU_8
    {CRC_MODE_CRC8, CRC_STANDARD_CRC_ITU, &crc_input, &expect_out[12]},
#endif
#if CRC_NONE_32
    {CRC_MODE_CRC32, CRC_STANDARD_CRC_NONE, &crc_input, &expect_out[13]},
#endif
#if CRC_PMEQ_32
    {CRC_MODE_CRC32, CRC_STANDARD_CRC_PMEQ_2, &crc_input, &expect_out[14]}
#endif

};

typedef struct {
    crc_mode_e mode;
    crc_standard_crc_e standard;
    int32_t expect_out;
} crc_config_test_t;
static crc_config_test_t crc_config_cases[] = {
    {CRC_MODE_CRC16, CRC_STANDARD_CRC_ROHC, (CSI_DRV_ERRNO_CRC_BASE | DRV_ERROR_PARAMETER)},
    {CRC_MODE_CRC8, CRC_STANDARD_CRC_MODBUS, (CSI_DRV_ERRNO_CRC_BASE | DRV_ERROR_PARAMETER)},
    {CRC_MODE_CRC8, CRC_STANDARD_CRC_IBM, (CSI_DRV_ERRNO_CRC_BASE | DRV_ERROR_PARAMETER)},
    {CRC_MODE_CRC8, CRC_STANDARD_CRC_USB, (CSI_DRV_ERRNO_CRC_BASE | DRV_ERROR_PARAMETER)},
    {CRC_MODE_CRC8, CRC_STANDARD_CRC_CCITT, (CSI_DRV_ERRNO_CRC_BASE | DRV_ERROR_PARAMETER)},
    {CRC_MODE_CRC8, CRC_STANDARD_CRC_X25, (CSI_DRV_ERRNO_CRC_BASE | DRV_ERROR_PARAMETER)}
};

typedef struct {
    uint32_t *input;
    int32_t expect_out;
} crc_calculate_test_t;

static crc_calculate_test_t crc_calculate_cases[] = {
    {NULL, (CSI_DRV_ERRNO_CRC_BASE | DRV_ERROR_PARAMETER)},
};

static void test_csi_crc_config(void)
{
    uint32_t ret = 0;
    uint32_t index;
    uint32_t i;
    uint32_t k;
    index = 1;

    for (i = 0; i < index; i++) {
        crc = csi_crc_initialize(i, crc_get_num_cb);
        ASSERT_TRUE(crc != NULL);

        for (k = 0; k < sizeof(crc_config_cases) / (sizeof(crc_config_test_t)); k++) {
            ret = csi_crc_config(crc, crc_config_cases[k].mode, crc_config_cases[k].standard);
            ASSERT_TRUE(ret == crc_config_cases[k].expect_out);
        }
    }

}

static void test_csi_crc_calculate(void)
{
    uint32_t ret = 0;
    uint32_t index;
    uint32_t i;
    uint32_t k;
    uint32_t out;
    index = 1;

    for (i = 0; i < index; i++) {
        crc = csi_crc_initialize(i, crc_get_num_cb);
        ASSERT_TRUE(crc != NULL);

        for (k = 0; k < sizeof(crc_calculate_cases) / (sizeof(crc_calculate_test_t)); k++) {
            ret = csi_crc_calculate(crc, crc_calculate_cases[k].input, &out, 1);
            ASSERT_TRUE(ret == crc_calculate_cases[k].expect_out);
        }
    }

}

static void test_crc_hash_calculate(void)
{
    uint32_t ret = 0;
    uint32_t index;
    uint32_t i;
    uint32_t k;
    uint32_t out;
    index = 1;

    for (i = 0; i < index; i++) {
        crc = csi_crc_initialize(i, crc_get_num_cb);
        ASSERT_TRUE(crc != NULL);

        for (k = 0; k < sizeof(crc_cases) / (sizeof(crc_test_t)); k++) {

            ret = csi_crc_config(crc, crc_cases[k].mode, crc_cases[k].standard);
            ASSERT_TRUE(ret == 0);
            ret = csi_crc_calculate(crc, crc_cases[k].input, &out, 4);
            ASSERT_TRUE(ret == 0);

            ASSERT_TRUE(out == *crc_cases[k].expect_out);

        }

        ret = csi_crc_uninitialize(crc);
        ASSERT_TRUE(ret == 0);

    }

}

/*****************************************************************************
test_crc: main function of the crc test

INPUT: NULL

RETURN: NULL

*****************************************************************************/
int test_crc()
{
    dtest_suite_info_t test_suite_info = {
        "test_crc", NULL, NULL, NULL, NULL
    };

    dtest_suite_t *test_suite = dtest_add_suite(&test_suite_info);

    dtest_case_info_t test_case_info_array[] = {
        { "test_crc_hash_calculate", test_crc_hash_calculate, CRC_FUNC_EN },
        { "test_csi_crc_calculate", test_csi_crc_calculate, CRC_CALCULATE_EN },
        { "test_csi_crc_config", test_csi_crc_config, CRC_CONFIG_EN },
        { NULL, NULL, 0 }
    };

    dtest_add_cases(test_suite, test_case_info_array);

    return 0;
}

int main(void)
{
    dtest_register_cb(test_crc);
    dtest();

    return 0;
}
