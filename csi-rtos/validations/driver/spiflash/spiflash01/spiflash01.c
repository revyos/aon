/*
 * Copyright (C) 2017-2019 Alibaba Group Holding Limited
 */

/******************************************************************************
 * @file     test_spiflash.c
 * @brief    the main function for the EFlash Driver
 * @version  V1.0
 * @date     20. July 2016
 ******************************************************************************/
#include "csi_config.h"
#include "dtest.h"
#include <stdio.h>
#include <stdbool.h>
#include "soc.h"
#include "drv/spiflash.h"
#include "test_driver_config.h"

#define SPIFLASH_ADDR_BASE  CSKY_QSPIMEM_BASE
#define OPERATE_ADDR    (SPIFLASH_ADDR_BASE + 0x80000)
#define OPERATE_LEN 0x1000

typedef struct {
    uint32_t addr;
    uint32_t len;
    uint32_t expect_out;

} spiflash_test_t;

static spiflash_test_t spiflash_erase_sector_cases[] = {
    {SPIFLASH_ADDR_BASE - 100, (uint32_t)NULL, (CSI_DRV_ERRNO_SPIFLASH_BASE | DRV_ERROR_PARAMETER)}
};

static spiflash_test_t spiflash_read_cases[] = {
    {SPIFLASH_ADDR_BASE, 0, (CSI_DRV_ERRNO_SPIFLASH_BASE | DRV_ERROR_PARAMETER)},
    {SPIFLASH_ADDR_BASE - 500, 5, (CSI_DRV_ERRNO_SPIFLASH_BASE | DRV_ERROR_PARAMETER)},
};

static spiflash_test_t spiflash_program_cases[] = {
    {SPIFLASH_ADDR_BASE, 0, (CSI_DRV_ERRNO_SPIFLASH_BASE | DRV_ERROR_PARAMETER)},
    {SPIFLASH_ADDR_BASE - 500, 5, (CSI_DRV_ERRNO_SPIFLASH_BASE | DRV_ERROR_PARAMETER)},
};

static spiflash_handle_t spiflash = NULL;

void test_pin_qspi_init(void)
{
#ifndef CONFIG_CHIP_DANICA
    csi_pin_set_mux(TEST_PIN_QSPI_MISO, TEST_PIN_QSPI_MISO_FUNC);
    csi_pin_set_mux(TEST_PIN_QSPI_MOSI, TEST_PIN_QSPI_MOSI_FUNC);
    csi_pin_set_mux(TEST_PIN_QSPI_CS, TEST_PIN_QSPI_CS_FUNC);
    csi_pin_set_mux(TEST_PIN_QSPI_SCK, TEST_PIN_QSPI_SCK_FUNC);
    csi_pin_set_mux(TEST_PIN_QSPI_WP, TEST_PIN_QSPI_WP_FUNC);
    csi_pin_set_mux(TEST_PIN_QSPI_HOLD, TEST_PIN_QSPI_HOLD_FUNC);
#endif
}


static void spiflash_get_num_cb(int32_t idx, spiflash_event_e event)
{
    printf("the spiflash calculate complete\r\n");
}

static void test_csi_spiflash_program(void)
{
    uint32_t index;
    int32_t ret;
    uint32_t i;
    uint32_t k;
    uint32_t source_data[5];
    index = 1;

    for (i = 0; i < index; i++) {
        spiflash = csi_spiflash_initialize(i, spiflash_get_num_cb);
        ASSERT_TRUE(spiflash != NULL);

        for (k = 0; k < sizeof(spiflash_erase_sector_cases) / (sizeof(spiflash_test_t)); k++) {
            ret = csi_spiflash_program(spiflash, spiflash_program_cases[k].addr, source_data, spiflash_erase_sector_cases[k].len);

            if (ret < 0) {
                ASSERT_TRUE(ret);
            }

            ASSERT_TRUE(ret == spiflash_erase_sector_cases[k].expect_out);
        }
    }
}

static void test_csi_spiflash_erase_sector(void)
{
    uint32_t index;
    int32_t ret;
    uint32_t i;
    uint32_t k;
    index = 1;

    for (i = 0; i < index; i++) {
        spiflash = csi_spiflash_initialize(i, spiflash_get_num_cb);
        ASSERT_TRUE(spiflash != NULL);

        for (k = 0; k < sizeof(spiflash_erase_sector_cases) / (sizeof(spiflash_test_t)); k++) {
            ret = csi_spiflash_erase_sector(spiflash, spiflash_erase_sector_cases[k].addr);

            if (ret < 0) {
                ASSERT_TRUE(ret);
            }

            ASSERT_TRUE(ret == spiflash_erase_sector_cases[k].expect_out);
        }
    }
}

static void test_csi_spiflash_read(void)
{
    uint32_t index;
    int32_t ret;
    uint32_t i;
    uint32_t k;
    uint8_t data[5];
    index = 1;

    test_pin_qspi_init();

    for (i = 0; i < index; i++) {
        spiflash = csi_spiflash_initialize(i, spiflash_get_num_cb);
        ASSERT_TRUE(spiflash != NULL);

        for (k = 0; k < sizeof(spiflash_read_cases) / (sizeof(spiflash_test_t)); k++) {
            ret = csi_spiflash_read(spiflash, spiflash_read_cases[k].addr, data, spiflash_read_cases[k].len);

            if (ret < 0) {
                ASSERT_TRUE(ret);
            }

            ASSERT_TRUE(ret == spiflash_read_cases[k].expect_out);
        }
    }
}


static void test_spiflash_get_info(void)
{
    spiflash_info_t *info = NULL;
    uint32_t index;
    int32_t ret;
    uint32_t i;
    index = 1;

    for (i = 0; i < index; i++) {
        spiflash = csi_spiflash_initialize(i, spiflash_get_num_cb);
        ASSERT_TRUE(spiflash != NULL);

        info = csi_spiflash_get_info(spiflash);
        ASSERT_TRUE(info != NULL);

        if (info != NULL) {
            printf("the index %u\n spiflash sector_count is %x \n sector_size is %x \n program_uint is %x\n erased value is %x\r\n", i, info->sector_count, info->sector_size, info->program_unit, info->erased_value);
        }

        ret = csi_spiflash_uninitialize(spiflash);
        ASSERT_TRUE(ret == 0);
    }
}

static uint32_t data[1024];
static void test_spiflash_erase(void)
{
    spiflash_info_t *info = NULL;
    uint32_t index;
    uint32_t erase_fail = 0;
    uint32_t addr = (OPERATE_ADDR);
    int32_t ret;
    uint32_t i;
    uint32_t j;
    uint32_t k;
    index = 1;

    for (i = 0; i < index; i++) {
        spiflash = csi_spiflash_initialize(i, spiflash_get_num_cb);
        ASSERT_TRUE(spiflash != NULL);

        info = csi_spiflash_get_info(spiflash);
        ASSERT_TRUE(info != NULL);

        for (j = 0; j < (OPERATE_LEN + info->sector_size - 1) / (info->sector_size); j++) {
            ret = csi_spiflash_erase_sector(spiflash, addr);
            ASSERT_TRUE(ret == 0);

            if (ret != 0) {
                printf("the ret is %x the addr is %x \r\n", ret, addr);
            }

            ret = csi_spiflash_read(spiflash, addr, data, info->sector_size);

            if (ret < 0) {
                ASSERT_TRUE(ret);
            }

            ASSERT_TRUE(ret == info->sector_size);

            for (k = 1; k < (info->sector_size >> 2); k++) {
                if (data[k] != data[k - 1]) {
                    break;
                }
            }

            ASSERT_TRUE(k == (info->sector_size >> 2));

            if (k != (info->sector_size >> 2)) {
                erase_fail = 1;
                printf("the index %u spiflash %x sector is erased failed\r\n", i, j);
            }

            addr = addr + info->sector_size;
        }

        ASSERT_TRUE(erase_fail == 0);

        if (erase_fail) {
            printf("the index %u spiflash erase failed\r\n", i);
        } else {
            printf("the index %u spiflash erase pass\r\n", i);
        }

        ret = csi_spiflash_uninitialize(spiflash);
        ASSERT_TRUE(ret == 0);
    }
}
static uint8_t source_data[4096];
static uint8_t dest_data[4096] = {0};

static void test_spiflash_program(void)
{
    spiflash_info_t *info = NULL;
    uint32_t index;
    uint32_t write_fail = 0;
    uint32_t addr = (OPERATE_ADDR);
    int32_t ret;
    uint32_t i;
    uint32_t j;
    uint32_t k;

    memset(source_data, 0x5a, sizeof(source_data));
    index = 1;

    for (i = 0; i < index; i++) {
        spiflash = csi_spiflash_initialize(i, spiflash_get_num_cb);
        ASSERT_TRUE(spiflash != NULL);

        info = csi_spiflash_get_info(spiflash);
        ASSERT_TRUE(info != NULL);

        for (j = 0; j < (OPERATE_LEN + info->sector_size - 1) / (info->sector_size); j++) {
            ret = csi_spiflash_program(spiflash, addr, source_data, info->sector_size);
            if (ret < 0) {
                ASSERT_TRUE(ret);
            }

            ASSERT_TRUE(ret == info->sector_size);

            ret = csi_spiflash_read(spiflash, addr, dest_data, info->sector_size);

            if (ret < 0) {
                ASSERT_TRUE(ret);
            }

            ASSERT_TRUE(ret == info->sector_size);

            for (k = 0; k < info->sector_size; k++) {
                if (dest_data[k] != source_data[k]) {
                    break;
                }
            }

            ASSERT_TRUE(k == info->sector_size);

            if (k != info->sector_size) {
                write_fail = 1;
                printf("the index %u spiflash %x sector is write failed\r\n", i, j);
            }

            addr = addr + info->sector_size;
        }

        ASSERT_TRUE(write_fail == 0);

        if (write_fail) {
            printf("the index %u spiflash write failed\r\n", i);
        } else {
            printf("the index %u spiflash write pass\r\n", i);
        }

        ret = csi_spiflash_uninitialize(spiflash);
        ASSERT_TRUE(ret == 0);
    }
}

/*****************************************************************************
test_spiflash: main function of the spiflash test

INPUT: NULL

RETURN: NULL

*****************************************************************************/
int test_spiflash()
{
    dtest_suite_info_t test_suite_info = {
        "test_spiflash", NULL, NULL, NULL, NULL
    };

    dtest_suite_t *test_suite = dtest_add_suite(&test_suite_info);

    dtest_case_info_t test_case_info_array[] = {
        { "test_spiflash_program", test_spiflash_program, SPIFLASH_PROGRAM_EN },
        { "test_spiflash_erase", test_spiflash_erase, SPIFLASH_ERASE_EN },
        { "test_spiflash_get_info", test_spiflash_get_info, SPIFLASH_GET_INFO_EN },
        { "test_csi_spiflash_program", test_csi_spiflash_program, SPIFLASH_PROGRAM_INTERFACE_EN },
        { "test_csi_spiflash_erase_sector", test_csi_spiflash_erase_sector, SPIFLASH_ERASE_SECTOR_INTERFACE_EN },
        { "test_csi_spiflash_read", test_csi_spiflash_read, SPIFLASH_READ_INTERFACE_EN },
        { NULL, NULL, 0 }
    };

    dtest_add_cases(test_suite, test_case_info_array);

    return 0;
}

int main(void)
{
    dtest_register_cb(test_spiflash);
    dtest();

    return 0;
}
