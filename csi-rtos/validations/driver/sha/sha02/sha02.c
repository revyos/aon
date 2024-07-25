/*
 * Copyright (C) 2017-2019 Alibaba Group Holding Limited
 */

/******************************************************************************
 * @file     task02.c
 * @brief    sha driver test
 * @version  V1.0
 * @date     14. Jun 2019
 ******************************************************************************/

#include "sha_test.h"
#include <string.h>
#include <stdio.h>
#include "dtest.h"

static uint8_t sha1_int_flag = 1;

static void sha_interrupt_handler(int32_t idx, sha_event_e event)
{
    if (event == SHA_EVENT_COMPLETE) {
        printf("SHA%d event cb entered\n", idx);
        sha1_int_flag = 0;
    }
}

void sha_if_test()
{
    //get capabilities
    sha_capabilities_t cap =  csi_sha_get_capabilities(2);
    ASSERT_TRUE(cap.sha1 == 0);

    char context[224];

    //initialize
    sha_handle_t sha = csi_sha_initialize(2, context, sha_interrupt_handler);
    ASSERT_TRUE(sha == NULL);

    sha = csi_sha_initialize(0, context, NULL);
    ASSERT_TRUE(sha != NULL);

    int32_t ret;
    char *input = "zth";
    char output[20];

    //power control
    ret = csi_sha_power_control(NULL, DRV_POWER_FULL);
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_SHA_BASE | DRV_ERROR_UNSUPPORTED));
    ret = csi_sha_power_control(sha, 10);
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_SHA_BASE | DRV_ERROR_UNSUPPORTED));

    //uninitialize
    ret = csi_sha_uninitialize(NULL);
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_SHA_BASE | DRV_ERROR_PARAMETER));

    //config
    ret = csi_sha_config(NULL, SHA_MODE_1, SHA_ENDIAN_MODE_BIG);
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_SHA_BASE | DRV_ERROR_PARAMETER));

    ret = csi_sha_config(sha, 8, SHA_ENDIAN_MODE_BIG);
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_SHA_BASE | SHA_ERROR_MODE));

    ret = csi_sha_config(sha, SHA_MODE_1, 2);
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_SHA_BASE | SHA_ERROR_ENDIAN));

    //start
    ret = csi_sha_start(NULL, context);
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_SHA_BASE | DRV_ERROR_PARAMETER));

    //ret = csi_sha_start(sha, NULL);
    //ASSERT_TRUE(ret == (CSI_DRV_ERRNO_SHA_BASE |DRV_ERROR_PARAMETER));

    ret = csi_sha_start(sha, context);

    //update
    ret = csi_sha_update(NULL, context, input, 3);
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_SHA_BASE | DRV_ERROR_PARAMETER));

    //ret = csi_sha_update(sha, NULL, input,3);
    //ASSERT_TRUE(ret == (CSI_DRV_ERRNO_SHA_BASE |DRV_ERROR_PARAMETER));

    ret = csi_sha_update(sha, context, NULL, 3);
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_SHA_BASE | DRV_ERROR_PARAMETER));

    ret = csi_sha_update(sha, context, input, 0);
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_SHA_BASE | DRV_ERROR_PARAMETER));

    ret = csi_sha_update(sha, context, input, 3);

    //finish
    ret = csi_sha_finish(NULL, context, output);
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_SHA_BASE | DRV_ERROR_PARAMETER));

    //ret = csi_sha_finish(sha, NULL, output);
    //ASSERT_TRUE(ret == (CSI_DRV_ERRNO_SHA_BASE |DRV_ERROR_PARAMETER));

    ret = csi_sha_finish(sha, context, NULL);
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_SHA_BASE | DRV_ERROR_PARAMETER));

    ret = csi_sha_finish(sha, context, output);

    sha_status_t stat = csi_sha_get_status(NULL);
    ASSERT_TRUE(stat.busy == 0);

    //get status
    sha_status_t status;
    status = csi_sha_get_status(NULL);
    ASSERT_TRUE(status.busy == 0);

    csi_sha_uninitialize(sha);
}

void sha_func_test()
{
    char *data = "zth";
    char expect[][64] = {
        {0x40, 0x32, 0xcc, 0xbb, 0x5e, 0x41, 0x15, 0x06, 0xcd, 0x6b, 0x8a, 0x9c, 0x36, 0xa9, 0x59, 0x29, 0xad, 0x2c, 0x41, 0x8d},
        {0x3e, 0x4e, 0x0f, 0xfd, 0x06, 0xfb, 0xb5, 0x31, 0x25, 0x05, 0xf3, 0xc1, 0x4b, 0x81, 0xe8, 0x12, 0xbd, 0xdb, 0x01, 0xe0, 0xc2, 0x11, 0x09, 0x80, 0x01, 0x22, 0xfa, 0xdd},
        {0xdb, 0x0f, 0x72, 0x5a, 0x1c, 0xc3, 0x91, 0x1b, 0x4a, 0xba, 0x9b, 0xf5, 0x0c, 0xef, 0x2c, 0x89, 0xa2, 0xe5, 0xa6, 0x4d, 0x6b, 0xd9, 0xa7, 0x1b, 0x4b, 0x30, 0xb9, 0xd4, 0xd4, 0x07, 0x13, 0x0e},
        {0x7b, 0xd3, 0xf5, 0xb8, 0x37, 0xf1, 0x02, 0xd0, 0x21, 0x83, 0xc1, 0x3b, 0x88, 0x8d, 0xcb, 0xef, 0x4d, 0xd9, 0xac, 0xcd, 0xb1, 0xca, 0xc7, 0x15, 0x6d, 0xd4, 0x43, 0x2a, 0x85, 0x33, 0x83, 0x71, 0x4d, 0xd2, 0x37, 0xff, 0xb9, 0x7a, 0x28, 0x9d, 0xce, 0x36, 0x35, 0x40, 0x9b, 0x3b, 0x22, 0xd0},
        {0x58, 0xe5, 0x28, 0xb9, 0x8a, 0x1b, 0x51, 0xeb, 0x74, 0x40, 0x50, 0x19, 0x4c, 0xe1, 0x94, 0x10, 0x92, 0xc8, 0xb2, 0x2f, 0xe8, 0x8f, 0xd3, 0x93, 0x49, 0x31, 0x2f, 0x17, 0xd2, 0x1b, 0x6d, 0x51, 0xec, 0x66, 0xcc, 0x8e, 0x28, 0xb4, 0xcd, 0x9c, 0x77, 0xe8, 0x98, 0xbd, 0xf9, 0x44, 0x15, 0x31, 0x6b, 0x02, 0x40, 0xc8, 0x47, 0x00, 0x92, 0xda, 0x5e, 0xbe, 0xb5, 0x87, 0xbe, 0x44, 0x65, 0x51},
    };

    char *context;
    char *context1;

    /*********** first three as parameters , last one as result ***********/
    sha_test_t check_list[] = {
        /*************** SHA-1 LE ************************/
        {data, SHA_MODE_1, SHA_ENDIAN_MODE_BIG, expect[0]},
        /*************** SHA-224 LE ************************/
//    {data, SHA_MODE_224, SHA_ENDIAN_MODE_BIG,expect[1]},
        /*************** SHA-256 LE ************************/
//    {data, SHA_MODE_256, SHA_ENDIAN_MODE_BIG,expect[2]},
        /*************** SHA-384 LE ************************/
//    {data, SHA_MODE_384, SHA_ENDIAN_MODE_BIG,expect[3]},
        /*************** SHA-512 LE ************************/
//    {data, SHA_MODE_512, SHA_ENDIAN_MODE_BIG,expect[4]},

    };

    int i = 0;
    uint32_t ret;
    char *out, *out1;
    int size[] = {20, 28, 32, 48, 64};
    sha_handle_t sha, sha1;

    sha_capabilities_t  cap = csi_sha_get_capabilities(0);

    if (cap.sha1) {
        printf("Support sha1 mode\n");
    }

    if (cap.sha224) {
        printf("Support sha224 mode\n");
    }

    if (cap.sha256) {
        printf("Support sha256 mode\n");
    }

    if (cap.sha384) {
        printf("Support sha384 mode\n");
    }

    if (cap.sha512) {
        printf("Support sha512 mode\n");
    }

    if (cap.sha512_224) {
        printf("Support sha512_224 mode\n");
    }

    if (cap.sha512_256) {
        printf("Support sha512_256 mode\n");
    }

    if (cap.endianmode) {
        printf("Support endian mode\n");
    }

    if (cap.interruptmode) {
        printf("Support interrupt mode\n");
    }

    context = (char *)malloc(224);
    context1 = (char *)malloc(224);

    /**********  Function Test   *********************/
    for (i = 0; i < sizeof(check_list) / sizeof(sha_test_t); i++) {

        sha = csi_sha_initialize(0, context, sha_interrupt_handler);
        ASSERT_TRUE(sha != NULL);
        ret = csi_sha_config(sha, check_list[i].sha_mode, check_list[i].endian);
        ASSERT_TRUE(ret == 0);

        out = malloc(size[i]);
        ret = csi_sha_start(sha, context);
        ASSERT_TRUE(ret == 0);

        sha1 = csi_sha_initialize(0, context1, sha_interrupt_handler);
        ASSERT_TRUE(sha1 != NULL);
        ret = csi_sha_config(sha1, check_list[i].sha_mode, check_list[i].endian);
        ASSERT_TRUE(ret == 0);

        out1 = malloc(size[i]);
        ret = csi_sha_start(sha1, context1);
        ASSERT_TRUE(ret == 0);

        ret = csi_sha_update(sha, context, data, 3);
        ASSERT_TRUE(ret == 0);

        ret = csi_sha_finish(sha, context, out);

        while (csi_sha_get_status(sha).busy == 1) ;

//        while(sha1_int_flag);
//        sha1_int_flag = 1;
        ASSERT_TRUE(memcmp(out, check_list[i].expect, size[i]) == 0);
        free(out);

        ret = csi_sha_update(sha1, context1, data, 3);
        ASSERT_TRUE(ret == 0);

        ret = csi_sha_finish(sha1, context1, out1);

        while (csi_sha_get_status(sha1).busy == 1) ;

//        while(sha1_int_flag);
//        sha1_int_flag = 1;
        ASSERT_TRUE(memcmp(out1, check_list[i].expect, size[i]) == 0);
        free(out1);

        csi_sha_uninitialize(sha);
        csi_sha_uninitialize(sha1);
    }

    free(context);
//    printf("Test finished\n");
}

int test_sha(void)
{
    dtest_suite_info_t test_suite_info = {
        "test_sha", NULL, NULL, NULL, NULL
    };

    dtest_suite_t *test_suite = dtest_add_suite(&test_suite_info);

    dtest_case_info_t test_case_info_array[] = {
        {"sha_if_test", sha_if_test, 1},
        {"sha_func_test", sha_func_test, 1},
        {NULL, NULL, 0}
    };

    dtest_add_cases(test_suite, test_case_info_array);

    return 0;
}

int main(void)
{
    dtest_register_cb(test_sha);
    dtest();

    return 0;
}
