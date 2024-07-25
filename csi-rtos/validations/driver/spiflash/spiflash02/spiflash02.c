/*
 * Copyright (C) 2017-2019 Alibaba Group Holding Limited
 */

/******************************************************************************
 * @file     spiflash02.c
 * @brief    the main function
 * @version  V1.0
 * @date     14. Jun 2019
 ******************************************************************************/

#include "spi_test.h"
#include "stdio.h"
#include <string.h>
#include <drv/timer.h>
#include "pin.h"
#include "dtest.h"

uint8_t spi_cb_flag = 1;
#define   TEST_SIZE    256
static void spi_event_cb_fun(int32_t idx, spiflash_event_e event)
{
    //if(event == SPI_EVENT_TX_COMPLETE)
    //printf("SPI%d event entered!\n",idx);
    spi_cb_flag = 0;
}

void spiflash_if_test()
{
#if 1
    //get_capabilities
    spiflash_capabilities_t cap = csi_spiflash_get_capabilities(2);
    ASSERT_TRUE(cap.erase_chip == 0);

    //initialize
    spiflash_handle_t spiflash = csi_spiflash_initialize(2, spi_event_cb_fun);
    ASSERT_TRUE(spiflash == NULL);

    spiflash = csi_spiflash_initialize(0, NULL);
    ASSERT_TRUE(spiflash != NULL);

    int32_t ret;
    //uninitialize
    ret = csi_spiflash_uninitialize(NULL);
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_SPIFLASH_BASE | DRV_ERROR_PARAMETER));

    //erase_chip
    ret = csi_spiflash_erase_chip(NULL);
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_SPIFLASH_BASE | DRV_ERROR_PARAMETER));

    //erase_sector
#endif

#if 0
    int32_t ret;
    ret = csi_spiflash_erase_sector(NULL, 0x10000000);
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_SPIFLASH_BASE | DRV_ERROR_PARAMETER));
#endif
#if 1
    char data[4] = {'a', 'b', 'c', 'd'};
    //program
    ret = csi_spiflash_program(NULL, 0x10000000, data, 4);
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_SPIFLASH_BASE | DRV_ERROR_PARAMETER));

    ret = csi_spiflash_program(spiflash, 0x10000000, NULL, 4);
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_SPIFLASH_BASE | DRV_ERROR_PARAMETER));

    char readdata[4];
    //read
    ret = csi_spiflash_read(NULL, 0x10000000, readdata, 4);
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_SPIFLASH_BASE | DRV_ERROR_PARAMETER));

    ret = csi_spiflash_read(spiflash, 0x10000000, NULL, 4);
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_SPIFLASH_BASE | DRV_ERROR_PARAMETER));

    //get_info
    spiflash_info_t *info = NULL;
    info = csi_spiflash_get_info(NULL);
    ASSERT_TRUE(info == NULL);

    //get_status
    spiflash_status_t stat;
    stat = csi_spiflash_get_status(NULL);
    ASSERT_TRUE(stat.busy == 0);
    ASSERT_TRUE(stat.error == 0);
#endif
}

void spiflash_func_test()
{
    int i, j;
    int32_t ret;
    spiflash_handle_t spiflash;
    uint32_t operate_addr, operate_sector_count;

    spiflash = csi_spiflash_initialize(0, spi_event_cb_fun);
    ASSERT_TRUE(spiflash != NULL);

    spiflash_info_t *info;
    info =  csi_spiflash_get_info(spiflash);

    printf("Start Address:%u\n", info->start);
    printf("End Address:%u\n", info->end);
    printf("Sector count:%u\n", info->sector_count);
    printf("Sector size:%u\n", info->sector_size);
    printf("Page size:%u\n", info->page_size);
    printf("Program unit:%u\n", info->program_unit);
    printf("Erased value:%u\n", info->erased_value);

    char data[TEST_SIZE], out[TEST_SIZE];

    operate_addr = info->start + USE_ADDR;
    operate_sector_count = USE_SEC - USE_ADDR / info->sector_size - 1;
    memset(data, 0x1, TEST_SIZE);

#if 0
    /****************  Case-0 spiflash erase sector  ************/
    memset(data, info->erased_value, TEST_SIZE);

    printf("Case0 started\n");

    for (i = 0; i < operate_sector_count; i++) {
        printf("the addr is %x\n", operate_addr);
        ret = csi_spiflash_erase_sector(spiflash, operate_addr);
        ASSERT_TRUE(ret == 0);
        csi_spiflash_read(spiflash, operate_addr, out, TEST_SIZE);
        ASSERT_TRUE(memcmp(data, out, TEST_SIZE) == 0);
        operate_addr += info->sector_size;
    }

    /*************** Case-1 spiflash write data  ************/
    printf("Case1 started\n");
    operate_addr = info->start + USE_ADDR;

    for (i = 0; i < operate_sector_count; i++) {
        ret = csi_spiflash_program(spiflash, operate_addr, data , TEST_SIZE);
        ASSERT_TRUE(ret == TEST_SIZE);
        operate_addr += info->sector_size;
    }

    /*****************  Case-2 Read data *************/
    printf("Case2 started\n");
    operate_addr = info->start + USE_ADDR;

    for (i = 0; i < operate_sector_count; i++) {
        ret = csi_spiflash_read(spiflash, operate_addr, out, TEST_SIZE);
        ASSERT_TRUE(ret  == TEST_SIZE);

        ASSERT_TRUE(memcmp(data, out, TEST_SIZE) == 0
    };
}
#endif

#if 0
/************* Case-3  Write data without erasing  ***********/
printf("Case3 started\n");
operate_addr = info->start + USE_ADDR;
ret = csi_spiflash_program(spiflash, operate_addr, data , TEST_SIZE);
ASSERT_TRUE(ret != 0);
#endif

#if 0
/************** Case-4  erase prgrame read for 1000 times  *********/
printf("Case4 started\n");
operate_addr = info->start + USE_ADDR;

for (i = 0; i < 1000; i++)
{
    ret = csi_spiflash_erase_sector(spiflash, operate_addr);
    ASSERT_TRUE(ret == 0);

    while (spi_cb_flag) ;

    spi_cb_flag = 1;
    ret = csi_spiflash_program(spiflash, operate_addr, data , TEST_SIZE);
    ASSERT_TRUE(ret == TEST_SIZE);
    ret = csi_spiflash_read(spiflash, operate_addr, out, TEST_SIZE);
    ASSERT_TRUE(ret == TEST_SIZE);

    ASSERT_TRUE(memcmp(data, out, TEST_SIZE) == 0);
}

#endif

//don't use
#if 0
/****************  Case-5 spiflash chip erase   ************/
printf("Case5 started\n");
ret = csi_spiflash_erase_chip(spiflash);
ASSERT_TRUE(ret == 0);

while (spi_cb_flag) ;

operate_addr = info->start + 0x80000;

for (i = 0; i < info->sector_count - 257; i++)
{
    ret = csi_spiflash_erase_sector(spiflash, operate_addr);
    ASSERT_TRUE(ret == 0);

    for (j = 0; j < info->sector_size; j += TEST_SIZE) {
        memset(out, 0 , TEST_SIZE);
        ret = csi_spiflash_read(spiflash, operate_addr, out, TEST_SIZE);

        ASSERT_TRUE(memcmp(data, out, TEST_SIZE) == 0);
    }

    operate_addr += info->sector_size;
}

spi_cb_flag = 1;
#endif

#if 0
/*********** Case-6  erase overflow  **************************/
printf("Case6 started\n");
operate_addr = info->end + 1 ;
ret = csi_spiflash_erase_sector(spiflash, operate_addr);
ASSERT_TRUE(ret != 0);

/********** Case-7 Write overflow  **************************/
printf("Case7 started\n");
ret = csi_spiflash_program(spiflash, operate_addr, data , TEST_SIZE);
ASSERT_TRUE(ret != 0);

/************  Case-8 Read overflow  ***********************/
printf("Case8 started\n");
ret = csi_spiflash_read(spiflash, operate_addr, out, TEST_SIZE);
ASSERT_TRUE(ret != 0);
#endif

#if 1
/**************** Preparing data   *****************************/

operate_addr = info->start + 0x80000 + info->sector_size - 0x8;
memset(data, 0x2, TEST_SIZE);
memset(out, 0x0, TEST_SIZE);

/*****************  Erase sector  ************************/
csi_spiflash_erase_sector(spiflash, operate_addr);

while (spi_cb_flag);

/************* Case-9 Write over sector  *************************/
printf("Case9 started\n");
ret = csi_spiflash_program(spiflash, operate_addr, data , TEST_SIZE);
ASSERT_TRUE(ret == TEST_SIZE);

/************* Case-10 Read over sector  *************************/
ret = csi_spiflash_read(spiflash, operate_addr, out, TEST_SIZE);
ASSERT_TRUE(ret == TEST_SIZE);
ASSERT_TRUE(memcmp(data, out, TEST_SIZE) == 0);
#endif

ret = csi_spiflash_uninitialize(spiflash);

printf("Test finished\n");
}
#define TRANS_LENGTH   1024

#if 0
void spi_perf_test()
{
    spi_cb_flag = 1;

    spi_handle_t spi = csi_spi_initialize(1, spi_event_cb_fun);
    timer_handle_t timer = csi_timer_initialize(0, NULL);

    uint32_t start, end;
    csi_timer_start(timer);
    int32_t i, ret;

    char data[TRANS_LENGTH];
    char out[TRANS_LENGTH];

    memset(out, 0 , TRANS_LENGTH);
    memset(data, 0x7f, TRANS_LENGTH);

    csi_spi_config(spi, 10000000, SPI_MODE_MASTER, SPI_FORMAT_CPOL0_CPHA0, SPI_ORDER_MSB2LSB, SPI_SS_MASTER_HW_OUTPUT, 8);

    csi_timer_get_current_value(timer, &start);

    for (i = 0; i < 1024; i++) {
        csi_spi_transfer(spi, data, out, TRANS_LENGTH, TRANS_LENGTH);

        while (spi_cb_flag);

        ret = memcmp(data, out, TRANS_LENGTH);

        if (ret != 0) {
            return ;
        }

        memset(out, 0 , TRANS_LENGTH);
        spi_cb_flag = 1;
    }

    csi_timer_get_current_value(timer, &end);

    printf("Test finished,start is %u, end is %u\n", start, end);

}
#endif


int test_spiflash(void)
{
    dtest_suite_info_t test_suite_info = {
        "test_spiflash", NULL, NULL, NULL, NULL
    };

    dtest_suite_t *test_suite = dtest_add_suite(&test_suite_info);

    dtest_case_info_t test_case_info_array[] = {
#if 0
        {"spiflash_if_test", spiflash_if_test, 1},
#endif
        {"spiflash_func_test", spiflash_func_test, 1},
        {NULL, NULL, 0}
    };

    dtest_add_cases(test_suite, test_case_info_array);

    return 0;
}

int main(void)
{
    /*
    csi_pin_set_mux(EXAMPLE_PIN_QSPI_MISO, EXAMPLE_PIN_QSPI_MISO_FUNC);
    csi_pin_set_mux(EXAMPLE_PIN_QSPI_MOSI, EXAMPLE_PIN_QSPI_MOSI_FUNC);
    csi_pin_set_mux(EXAMPLE_PIN_QSPI_CS, EXAMPLE_PIN_QSPI_CS_FUNC);
    csi_pin_set_mux(EXAMPLE_PIN_QSPI_SCK, EXAMPLE_PIN_QSPI_SCK_FUNC);
    csi_pin_set_mux(EXAMPLE_PIN_QSPI_WP, EXAMPLE_PIN_QSPI_WP_FUNC);
    csi_pin_set_mux(EXAMPLE_PIN_QSPI_HOLD, EXAMPLE_PIN_QSPI_HOLD_FUNC);
    */

    dtest_register_cb(test_spiflash);
    dtest();

    return 0;
}
