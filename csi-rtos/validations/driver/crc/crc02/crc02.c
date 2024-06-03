/*
 * Copyright (C) 2017-2019 Alibaba Group Holding Limited
 */

/******************************************************************************
 * @file     crc02.c
 * @brief    the main function
 * @version  V1.0
 * @date     14. Jun 2019
 ******************************************************************************/

#include "crc_test.h"
#include <string.h>
#include <stdio.h>
#include "dtest.h"

void crc_cb_func()
{

}

void crc_if_test()
{
    //get_capabilities
    crc_capabilities_t cap = csi_crc_get_capabilities(2);
    ASSERT_TRUE(cap.ROHC == 0);

    //initialize
    crc_handle_t crc =  csi_crc_initialize(2, crc_cb_func);
    ASSERT_TRUE(crc == NULL);

    crc = csi_crc_initialize(0, crc_cb_func);
    ASSERT_TRUE(crc != NULL);

    int32_t ret;
    //uninitialize
    ret = csi_crc_uninitialize(NULL);
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_CRC_BASE | DRV_ERROR_PARAMETER));

    //config
    ret = csi_crc_config(NULL, CRC_MODE_CRC16, CRC_STANDARD_CRC_ROHC);
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_CRC_BASE | DRV_ERROR_PARAMETER));

    ret = csi_crc_config(crc, 3, CRC_STANDARD_CRC_ROHC);
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_CRC_BASE | CRC_ERROR_MODE));

    ret = csi_crc_config(crc, CRC_MODE_CRC16, 7);
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_CRC_BASE | DRV_ERROR_PARAMETER));

    ret = csi_crc_config(crc, CRC_MODE_CRC16, CRC_STANDARD_CRC_ROHC);
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_CRC_BASE | DRV_ERROR_PARAMETER));

    char *key = "1234567890abcdef";
    char out[2];

    //calculate
    ret = csi_crc_calculate(NULL, key, out, 16);
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_CRC_BASE | DRV_ERROR_PARAMETER));

    ret = csi_crc_calculate(crc, NULL, out, 16);
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_CRC_BASE | DRV_ERROR_PARAMETER));

    ret = csi_crc_calculate(crc, key, NULL, 16);
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_CRC_BASE | DRV_ERROR_PARAMETER));

    ret = csi_crc_calculate(crc, key, out, 0);
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_CRC_BASE | DRV_ERROR_PARAMETER));

    //get_status
    crc_status_t stat = csi_crc_get_status(NULL);
    ASSERT_TRUE(stat.busy == 0);

    ret = csi_crc_uninitialize(crc);
}

void crc_func_test()
{

    char *key = "1234567890abcdef";
    char expect[][2] = {
        {0x9c},
        {0x9},
        {0xc2, 0xd3},
        {0x3d, 0x2c},
        {0x83, 0xdc},
        {0x7c, 0x23},
        {0x5c, 0x63},
        {0xf5, 0xcc},
    };

    /*********** first three as parameters , last one as result ***********/
    crc_test_t check_list[] = {
        /**********  CRC8 - ROHC *********/
        {key, CRC_MODE_CRC8, CRC_STANDARD_CRC_ROHC, expect[0]},
        /**********  CRC8 - MAXIM *********/
        {key, CRC_MODE_CRC8, CRC_STANDARD_CRC_MAXIM, expect[1]},
        /**********  CRC16 - IBM *********/
        {key, CRC_MODE_CRC16, CRC_STANDARD_CRC_IBM, expect[2]},
        /**********  CRC16 - MAXIM *********/
        {key, CRC_MODE_CRC16, CRC_STANDARD_CRC_MAXIM, expect[3]},
        /**********  CRC16 - USB *********/
        {key, CRC_MODE_CRC16, CRC_STANDARD_CRC_USB, expect[4]},
        /**********  CRC16 - MODBUS *********/
        {key, CRC_MODE_CRC16, CRC_STANDARD_CRC_MODBUS, expect[5]},
        /**********  CRC16 - CCITT *********/
        {key, CRC_MODE_CRC16, CRC_STANDARD_CRC_CCITT, expect[6]},
        /**********  CRC16 - X25 *********/
        {key, CRC_MODE_CRC16, CRC_STANDARD_CRC_X25, expect[7]},
    };

    int i = 0, count;
    uint32_t ret;
    char *out;
    crc_handle_t crc;

    crc_status_t status;
    status.busy = 1;

    crc = csi_crc_initialize(0, NULL);

    if (crc == NULL) {
        printf("Fail to init CRC, this test failed cannot be continued!\n");
        return;
    }

    /**********  Function Test   *********************/
    for (i = 0; i < sizeof(check_list) / sizeof(crc_test_t); i++) {
        if (i < 2) {
            count = 1;
        } else {
            count = 2;
        }

        ret = csi_crc_config(crc, check_list[i].crc_mode, check_list[i].crc_std);
        ASSERT_TRUE(ret == 0);

        out = malloc(count);

        ret = csi_crc_calculate(crc, check_list[i].key, (void *)out, 16);

        while (status.busy == 1) {
            status = csi_crc_get_status(crc);
        }

        ASSERT_TRUE(ret == 0);

        ASSERT_TRUE(memcmp(out, check_list[i].expect, count) == 0);
        free(out);
    }

    csi_crc_uninitialize(crc);
    printf("Test finished\n");
}

int test_crc(void)
{
    dtest_suite_info_t test_suite_info = {
        "test_crc", NULL, NULL, NULL, NULL
    };

    dtest_suite_t *test_suite = dtest_add_suite(&test_suite_info);

    dtest_case_info_t test_case_info_array[] = {
        {"crc_if_test", crc_if_test, 1},
        {"crc_func_test", crc_func_test, 1},
        {NULL, NULL, 0}
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
