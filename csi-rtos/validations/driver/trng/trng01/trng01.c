/*
 * Copyright (C) 2017-2019 Alibaba Group Holding Limited
 */

/******************************************************************************
 * @file     test_trng.c
 * @brief    the test function for the trng driver
 * @version  V1.0
 * @date     20. July 2016
 ******************************************************************************/
#include "dtest.h"
#include "soc.h"
#include "drv/rng.h"
#include "stdio.h"
#include "test_driver_config.h"

#define NUM     20

static trng_handle_t trng = NULL;

static void trng_get_num_cb(int32_t idx, trng_event_e event)
{
    printf("the data genetate complete\r\n");
}

typedef struct {
    uint8_t *result;
    uint32_t len;
    uint32_t expect_out;
} trng_test_t;

static int32_t data;

static trng_test_t trng_get_data_cases[] = {
    {NULL, 4, (CSI_DRV_ERRNO_TRNG_BASE | DRV_ERROR_PARAMETER)},
    {(uint8_t *) &data, 0, (CSI_DRV_ERRNO_TRNG_BASE | DRV_ERROR_PARAMETER)}
};
static void test_csi_trng_get_data(void)
{
    int ret = 0;
    int i;
    int k;
    int index = 0;
    index = 1;
    ASSERT_TRUE(index >= 0);

    for (k = 0; k < index; k++) {
        trng = csi_trng_initialize(k, trng_get_num_cb);
        ASSERT_TRUE(trng != NULL);

        for (i = 0; i < sizeof(trng_get_data_cases) / (sizeof(trng_test_t)); i++) {
            ret = csi_trng_get_data(trng, trng_get_data_cases[i].result, trng_get_data_cases[i].len);
            ASSERT_TRUE(ret == trng_get_data_cases[i].expect_out);
        }
    }

}

static void test_get_num(void)
{
    int ret = 0;
    int i;
    int j;
    int k;
    uint8_t  data[NUM] = {0x0};
    trng_status_t status;
    int index = 0;
    index = 1;
    ASSERT_TRUE(index >= 0);

    if (index >= 0) {
        for (k = 0; k < index; k++) {
            trng = csi_trng_initialize(k, trng_get_num_cb);
            ASSERT_TRUE(trng != NULL);

            ret = csi_trng_get_data(trng, data, NUM);
            ASSERT_TRUE(ret == 0);

            while (1) {
                status = csi_trng_get_status(trng);

                if (status.busy == 0 && status.data_valid == 1) {
                    ret = csi_trng_uninitialize(trng);
                    ASSERT_TRUE(ret == 0);
                    break;
                }
            }

            for (i = 1; i < NUM; i++) {
                for (j = 0; j < i; j++) {
                    if (data[i] == data[j]) {
                        break;
                    }
                }
            }

            ASSERT_TRUE(i == NUM);

            if (i != NUM) {
                printf("the index %d trng get num failed\r\n", k);
            } else {
                printf("the index %d trng get num passed\r\n", k);
            }
        }
    }

}

/*****************************************************************************
test_trng: main function of the trng test

INPUT: NULL

RETURN: NULL

*****************************************************************************/
int test_trng()
{
    dtest_suite_info_t test_suite_info = {
        "test_trng", NULL, NULL, NULL, NULL
    };

    dtest_suite_t *test_suite = dtest_add_suite(&test_suite_info);

    dtest_case_info_t test_case_info_array[] = {
        { "test_get_num", test_get_num, TRNG_GET_NUM_EN },
        { "test_csi_trng_get_data", test_csi_trng_get_data, TRNG_GET_DATA_INTERFACE_EN },
        { NULL, NULL, 0 }
    };

    dtest_add_cases(test_suite, test_case_info_array);

    return 0;
}

int main(void)
{
    dtest_register_cb(test_trng);
    dtest();

    return 0;
}
