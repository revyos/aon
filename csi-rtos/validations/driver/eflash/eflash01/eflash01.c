/*
 * Copyright (C) 2017-2019 Alibaba Group Holding Limited
 */

/******************************************************************************
 * @file     test_eflash.c
 * @brief    the main function for the EFlash Driver
 * @version  V1.0
 * @date     20. July 2016
 ******************************************************************************/
#include "dtest.h"
#include <stdio.h>
#include <stdbool.h>
#include "csi_config.h"
#include "soc.h"
#include "drv/eflash.h"
#include "test_driver_config.h"


#ifdef CONFIG_CHIP_ZX297100
#define TEST_EFLASH_BASE    ZX29_EFLASH_BASE
static uint32_t index = 1;
#else
#define TEST_EFLASH_BASE    CSKY_EFLASH_BASE
static uint32_t index = 0;
#endif

#define EFLASH_TEST_SIZE    0x200

static const uint8_t eflash_test[EFLASH_TEST_SIZE] __attribute__((aligned(512)))={0x0};

typedef struct {
    uint32_t addr;
    uint32_t len;
    uint32_t expect_out;

} eflash_test_t;

static eflash_test_t eflash_erase_sector_cases[] = {
    {TEST_EFLASH_BASE - 100, NULL, (CSI_DRV_ERRNO_EFLASH_BASE | DRV_ERROR_PARAMETER)}
};

static eflash_test_t eflash_read_cases[] = {
    {TEST_EFLASH_BASE, 0, (CSI_DRV_ERRNO_EFLASH_BASE | DRV_ERROR_PARAMETER)},
    {TEST_EFLASH_BASE - 500, 5, (CSI_DRV_ERRNO_EFLASH_BASE | DRV_ERROR_PARAMETER)},
};

static eflash_test_t eflash_program_cases[] = {
    {TEST_EFLASH_BASE, 0, (CSI_DRV_ERRNO_EFLASH_BASE | DRV_ERROR_PARAMETER)},
    {TEST_EFLASH_BASE - 500, 5, (CSI_DRV_ERRNO_EFLASH_BASE | DRV_ERROR_PARAMETER)},
};

static eflash_handle_t eflash = NULL;

static void eflash_get_num_cb(int32_t idx, eflash_event_e event)
{
    printf("the eflash calculate complete\r\n");
}

static void test_csi_eflash_program(void)
{
    uint32_t ret;
    uint32_t k;
    uint32_t source_data[5];

    eflash = csi_eflash_initialize(index, eflash_get_num_cb);
    ASSERT_TRUE(eflash != NULL);

    for (k = 0; k < sizeof(eflash_erase_sector_cases) / (sizeof(eflash_test_t)); k++) {
        ret = csi_eflash_program(eflash, eflash_program_cases[k].addr, source_data, eflash_erase_sector_cases[k].len);
        ASSERT_TRUE(ret == eflash_erase_sector_cases[k].expect_out);
    }
}

static void test_csi_eflash_erase_sector(void)
{
    uint32_t ret;
    uint32_t k;

    eflash = csi_eflash_initialize(index, eflash_get_num_cb);
    ASSERT_TRUE(eflash != NULL);

    for (k = 0; k < sizeof(eflash_erase_sector_cases) / (sizeof(eflash_test_t)); k++) {
        ret = csi_eflash_erase_sector(eflash, eflash_erase_sector_cases[k].addr);
        ASSERT_TRUE(ret == eflash_erase_sector_cases[k].expect_out);
    }
}

static void test_csi_eflash_read(void)
{
    uint32_t ret;
    uint32_t k;
    uint8_t data[5];

    eflash = csi_eflash_initialize(index, eflash_get_num_cb);
    ASSERT_TRUE(eflash != NULL);

    for (k = 0; k < sizeof(eflash_read_cases) / (sizeof(eflash_test_t)); k++) {
        ret = csi_eflash_read(eflash, eflash_read_cases[k].addr, data, eflash_read_cases[k].len);
        ASSERT_TRUE(ret == eflash_read_cases[k].expect_out);
    }
}


static void test_eflash_get_info(void)
{
    eflash_info_t *info = NULL;
    uint32_t ret;

    eflash = csi_eflash_initialize(index, eflash_get_num_cb);
    ASSERT_TRUE(eflash != NULL);

    info = csi_eflash_get_info(eflash);
    ASSERT_TRUE(info != NULL);

    if (info != NULL) {
        printf("the index %u\n eflash sector_count is %x \n sector_size is %x \n program_uint is %x\n erased value is %x\r\n", index, info->sector_count, info->sector_size, info->program_unit, info->erased_value);
    }

    ret = csi_eflash_uninitialize(eflash);
    ASSERT_TRUE(ret == 0);
}

static void test_eflash_erase(void)
{
    eflash_info_t *info = NULL;
    uint32_t erase_fail = 0;
    uint32_t data[128];
    uint32_t operate_addr = (uint32_t)&eflash_test;
    uint32_t operate_len = EFLASH_TEST_SIZE;
    uint32_t ret;
    uint32_t j;
    uint32_t k;

    eflash = csi_eflash_initialize(index, eflash_get_num_cb);
    ASSERT_TRUE(eflash != NULL);

    info = csi_eflash_get_info(eflash);
    ASSERT_TRUE(info != NULL);

    uint32_t sector_num = (operate_addr - (operate_addr & ~(info->sector_size -1)) + operate_len + info->sector_size -1 ) / info->sector_size;

    for (j = 0; j < sector_num; j++) {
        ret = csi_eflash_erase_sector(eflash, operate_addr);
        ASSERT_TRUE(ret == 0);

        if (ret != 0) {
            printf("the ret is %x the addr is %x \r\n", ret, operate_addr);
        }

        ret = csi_eflash_read(eflash, operate_addr & ~(info->sector_size -1), data, info->sector_size);
        ASSERT_TRUE(ret == info->sector_size);

        for (k = 1; k < (info->sector_size >> 2); k++) {
            if (data[k] != data[k - 1]) {
                break;
            }
        }

        ASSERT_TRUE(k == (info->sector_size >> 2));

        if (k != (info->sector_size >> 2)) {
            erase_fail = 1;
            printf("the index %u eflash %x sector is erased failed\r\n", index, j);
        }

        operate_addr = operate_addr + info->sector_size;
    }

    ASSERT_TRUE(erase_fail == 0);

    if (erase_fail) {
        printf("the index %u eflash erase failed\r\n", index);
    } else {
        printf("the index %u eflash erase pass\r\n", index);
    }

    ret = csi_eflash_uninitialize(eflash);
    ASSERT_TRUE(ret == 0);
}

static void test_eflash_program(void)
{
    eflash_info_t *info = NULL;
    uint32_t write_fail = 0;
    uint8_t source_data[512];
    uint8_t dest_data[512] = {0};
    uint32_t operate_addr = (uint32_t)&eflash_test;
    uint32_t operate_len = EFLASH_TEST_SIZE;
    uint32_t ret;
    uint32_t j;
    uint32_t k;

    memset(source_data, 0x5a, sizeof(source_data));

    eflash = csi_eflash_initialize(index, eflash_get_num_cb);
    ASSERT_TRUE(eflash != NULL);

    info = csi_eflash_get_info(eflash);
    ASSERT_TRUE(info != NULL);

    uint32_t sector_num = (operate_addr - (operate_addr & ~(info->sector_size -1)) + operate_len + info->sector_size -1 ) / info->sector_size;

    for (j = 0; j < sector_num; j++) {
        ret = csi_eflash_program(eflash, operate_addr, source_data, info->sector_size);
        ASSERT_TRUE(ret == info->sector_size);

        ret = csi_eflash_read(eflash, operate_addr, dest_data, info->sector_size);
        ASSERT_TRUE(ret == info->sector_size);

        for (k = 0; k < info->sector_size; k++) {
            if (dest_data[k] != source_data[k]) {
                break;
            }
        }

        ASSERT_TRUE(k == info->sector_size);

        if (k != info->sector_size) {
            write_fail = 1;
            printf("the index %u eflash %x sector is write failed\r\n", index, j);
        }

        operate_addr += info->sector_size;
        operate_len -= info->sector_size;
    }

    if (operate_len) {
        csi_eflash_program(eflash, operate_addr, source_data, operate_len);

        csi_eflash_read(eflash, operate_addr, dest_data, operate_len);

        for (k = 0; k < operate_len; k++) {
            if (dest_data[k] != source_data[k]) {
                break;
            }
        }

        if (k != operate_len) {
            write_fail = 1;
        }
    }

    ASSERT_TRUE(write_fail == 0);

    if (write_fail) {
        printf("the index %u eflash write failed\r\n", index);
    } else {
        printf("the index %u eflash write pass\r\n", index);
    }

    ret = csi_eflash_uninitialize(eflash);
    ASSERT_TRUE(ret == 0);
}

/*****************************************************************************
test_eflash: main function of the eflash test

INPUT: NULL

RETURN: NULL

*****************************************************************************/
int test_eflash()
{
    dtest_suite_info_t test_suite_info = {
        "test_eflash", NULL, NULL, NULL, NULL
    };

    dtest_suite_t *test_suite = dtest_add_suite(&test_suite_info);

    dtest_case_info_t test_case_info_array[] = {
        { "test_eflash_program", test_eflash_program, EFLASH_PROGRAM_EN },
        { "test_eflash_erase", test_eflash_erase, EFLASH_ERASE_EN },
        { "test_eflash_get_info", test_eflash_get_info, EFLASH_GET_INFO_EN },
        { "test_csi_eflash_program", test_csi_eflash_program, EFLASH_PROGRAM_INTERFACE_EN },
        { "test_csi_eflash_erase_sector", test_csi_eflash_erase_sector, EFLASH_ERASE_SECTOR_INTERFACE_EN },
        { "test_csi_eflash_read", test_csi_eflash_read, EFLASH_READ_INTERFACE_EN },
        { NULL, NULL, 0 }
    };

    dtest_add_cases(test_suite, test_case_info_array);

    return 0;
}

int main(void)
{
    dtest_register_cb(test_eflash);
    dtest();

    return 0;
}
