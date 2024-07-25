/*
 * Copyright (C) 2017-2019 Alibaba Group Holding Limited
 */

/******************************************************************************
 * @file     spi02.c
 * @brief    the main function
 * @version  V1.0
 * @date     14. Jun 2019
 ******************************************************************************/

#include "spi_test.h"
#include "stdio.h"
#include <string.h>
#include "spi_test1.h"

static void spi_event_cb_fun(int32_t idx, spi_event_e event)
{
    //printf("SPI event entered!\n");
}

void spi_if_test()
{
    //get_capabilities
    spi_capabilities_t cap = csi_spi_get_capabilities(5);
    ASSERT_TRUE(cap.simplex == 0);

    //initialize
    spi_handle_t spi = csi_spi_initialize(5, spi_event_cb_fun);
    ASSERT_TRUE(spi == NULL);

    spi = csi_spi_initialize(0, NULL);
    ASSERT_TRUE(spi != NULL);

    int32_t ret;
    //power_control
    ret = csi_spi_power_control(NULL, DRV_POWER_FULL);
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_SPI_BASE | DRV_ERROR_UNSUPPORTED));
    ret = csi_spi_power_control(spi, 5);
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_SPI_BASE | DRV_ERROR_UNSUPPORTED));

    //uninitialize
    ret = csi_spi_uninitialize(NULL);
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_SPI_BASE | DRV_ERROR_PARAMETER));

    //config_mode
    ret = csi_spi_config_mode(NULL, SPI_MODE_MASTER);
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_SPI_BASE | DRV_ERROR_PARAMETER));

    ret = csi_spi_config_mode(spi, 5);
    //ASSERT_TRUE(ret == (CSI_DRV_ERRNO_SPI_BASE | DRV_ERROR_UNSUPPORTED));
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_SPI_BASE | SPI_ERROR_MODE));

    ret = csi_spi_config_mode(spi, -1);
    //ASSERT_TRUE(ret == (CSI_DRV_ERRNO_SPI_BASE | DRV_ERROR_UNSUPPORTED));
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_SPI_BASE | SPI_ERROR_MODE));

    //config_block_mode
    ret = csi_spi_config_block_mode(NULL, 0);
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_SPI_BASE | DRV_ERROR_PARAMETER));

    ret = csi_spi_config_block_mode(spi, -1);
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_SPI_BASE | DRV_ERROR_PARAMETER));

    //config_baudrate
    ret = csi_spi_config_baudrate(NULL, 115200);
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_SPI_BASE | DRV_ERROR_PARAMETER));

    //config_bit_order
    ret = csi_spi_config_bit_order(NULL, SPI_ORDER_MSB2LSB);
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_SPI_BASE | DRV_ERROR_PARAMETER));

    ret = csi_spi_config_bit_order(spi, 2);
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_SPI_BASE | SPI_ERROR_BIT_ORDER));

    ret = csi_spi_config_bit_order(spi, -1);
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_SPI_BASE | SPI_ERROR_BIT_ORDER));

    //config_datawidth
    ret = csi_spi_config_datawidth(NULL, 32);
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_SPI_BASE | DRV_ERROR_PARAMETER));

    ret = csi_spi_config_datawidth(spi, 0);
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_SPI_BASE | SPI_ERROR_DATA_BITS));

    ret = csi_spi_config_datawidth(spi, 33);
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_SPI_BASE | SPI_ERROR_DATA_BITS));

    ret = csi_spi_config_datawidth(spi, -1);
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_SPI_BASE | SPI_ERROR_DATA_BITS));

    //config_format
    ret = csi_spi_config_format(NULL, SPI_FORMAT_CPOL0_CPHA0);
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_SPI_BASE | DRV_ERROR_PARAMETER));

    ret = csi_spi_config_format(spi, 4);
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_SPI_BASE | SPI_ERROR_FRAME_FORMAT));

    ret = csi_spi_config_format(spi, -1);
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_SPI_BASE | SPI_ERROR_FRAME_FORMAT));

    //config_ss_mode
    ret = csi_spi_config_ss_mode(NULL, SPI_SS_MASTER_UNUSED);
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_SPI_BASE | DRV_ERROR_PARAMETER));

    ret = csi_spi_config_ss_mode(spi, 6);
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_SPI_BASE | SPI_ERROR_SS_MODE));

    ret = csi_spi_config_ss_mode(spi, -1);
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_SPI_BASE | SPI_ERROR_SS_MODE));

    uint8_t data[8] = {1, 1, 1, 1, 1, 1, 1, 1};
    //send
    ret = csi_spi_send(NULL, data, 8);
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_SPI_BASE | DRV_ERROR_PARAMETER));

    ret = csi_spi_send(spi, NULL, 8);
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_SPI_BASE | DRV_ERROR_PARAMETER));

    ret = csi_spi_send(spi, data, 0);
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_SPI_BASE | DRV_ERROR_PARAMETER));

    ret = csi_spi_send(spi, data, 8);

    uint8_t recv[8];
    //receive
    ret = csi_spi_receive(NULL, recv, 8);
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_SPI_BASE | DRV_ERROR_PARAMETER));

    ret = csi_spi_receive(spi, NULL, 8);
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_SPI_BASE | DRV_ERROR_PARAMETER));

    ret = csi_spi_receive(spi, recv, 0);
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_SPI_BASE | DRV_ERROR_PARAMETER));

    //ss_control
    ret = csi_spi_ss_control(NULL, SPI_SS_ACTIVE);
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_SPI_BASE | DRV_ERROR_PARAMETER));

    ret = csi_spi_ss_control(spi, 2);
    ASSERT_TRUE(ret == (CSI_DRV_ERRNO_SPI_BASE | DRV_ERROR_PARAMETER));

}

void spi_func_test()
{
    uint8_t id[3] = {0x11, 0x11};
    int i = 0;
    int32_t ret;
    spi_handle_t spiflash;
    uint32_t operate_addr;

    /*
    csi_pin_set_mux(PA18, EXAMPLE_PIN_SPI_MISO_FUNC);
    csi_pin_set_mux(PA17, EXAMPLE_PIN_SPI_MOSI_FUNC);
    csi_pin_set_mux(PA16, EXAMPLE_PIN_SPI_CS_FUNC);
    csi_pin_set_mux(PA19, EXAMPLE_PIN_SPI_SCK_FUNC);
    */

    csi_pin_set_mux(SPI_TX, SPI_TX_FUNC);
    csi_pin_set_mux(SPI_RX, SPI_RX_FUNC);
    csi_pin_set_mux(SPI_CLK, SPI_CLK_FUNC);
    csi_pin_set_mux(SPI_CS, SPI_CS_FUNC);

    spiflash = csi_spi_initialize(SPI_INDEX, (spi_event_cb_t)spi_event_cb_fun);
    ASSERT_TRUE(spiflash != NULL);

    ret = w25q64flash_read_id(spiflash, (uint32_t *)&id);
    ASSERT_TRUE(ret == 0);

    char erase_value[256], *data, *out;

    memset(erase_value, SPIFLASH_ERASE_VALUE, 256);
    /****************  Case-0 spiflash erase sector  ************/

    printf("Case0 started\n");
    operate_addr = SPIFLASH_BASE_ADDR;
    out = (char *)malloc(4);

    for (i = 0; i < SECTOR_COUNT; i++) {
        ret = w25q64flash_erase_sector(spiflash, operate_addr);
        ASSERT_TRUE(ret == 0);
        w25q64flash_read_data(spiflash, operate_addr, out, 4);
        ASSERT_TRUE(memcmp(erase_value, out, 4) == 0);
        operate_addr += SECTOR_SIZE;
    }

    /*************** Case-1 spiflash write data  ************/
    printf("Case1 started\n");
    data = (char *)malloc(256);
    memset(data, 0x1, 256);
    operate_addr = SPIFLASH_BASE_ADDR;

    for (i = 0; i < SECTOR_COUNT; i++) {
        ret = w25q64flash_program_data(spiflash, operate_addr, data , 256);
        ASSERT_TRUE(ret == 256);
        operate_addr += SECTOR_SIZE;
    }

    free(out);

    /*****************  Case-2 Read data *************/
    printf("Case2 started\n");
    out = (char *)malloc(256);
    operate_addr = SPIFLASH_BASE_ADDR;

    for (i = 0; i < SECTOR_COUNT; i++) {
        ret = w25q64flash_read_data(spiflash, operate_addr, out, 256);
        ASSERT_TRUE(ret  == 256);

        ASSERT_TRUE(memcmp(data, out, 256) == 0);
    }

    /************* Case-3  Write data without erasing  ***********/
    printf("Case3 started\n");
    ret = w25q64flash_program_data(spiflash, operate_addr, data , 256);
    ASSERT_TRUE(ret != 0);

    /************** Case-4  erase prgrame read for 1000 times  *********/
    /*    printf("Case4 started\n");
        operate_addr = SPIFLASH_BASE_ADDR;
        for(i=0;i<1000;i++)
        {
            ret = w25q64flash_erase_sector(spiflash,operate_addr);
            ASSERT_TRUE(ret == 0);
            ret = w25q64flash_program_data(spiflash, operate_addr, data , 256);
            ASSERT_TRUE(ret == 256);
            ret = w25q64flash_read_data(spiflash,operate_addr, out, 256);
            ASSERT_TRUE(ret == 256);

            ASSERT_TRUE(memcmp(data, out, 256) == 0);
        }
    */
    /****************  Case-5 spiflash chip erase   ************/
    /*    printf("Case5 started\n");
        ret = w25q64flash_erase_chip(spiflash);
        ASSERT_TRUE(ret == 0);
        operate_addr = SPIFLASH_BASE_ADDR;
        for(i=0;i<1;i++) //SECTOR_COUNT
        {
            ret = w25q64flash_read_data(spiflash,operate_addr, out, 256);

            ASSERT_TRUE(memcmp(erase_value, out, 256) == 0);
            operate_addr += SECTOR_SIZE;
        }
    */
    /*********** Case-6  erase overflow  **************************/
    printf("Case6 started\n");
    operate_addr = SPIFLASH_BASE_ADDR + SECTOR_COUNT * SECTOR_SIZE ;
    ret = w25q64flash_erase_sector(spiflash, operate_addr);
    ASSERT_TRUE(ret != 0);

    /********** Case-7 Write overflow  **************************/
    printf("Case7 started\n");
    ret = w25q64flash_program_data(spiflash, operate_addr, data , 256);
    ASSERT_TRUE(ret != 0);

    /************  Case-8 Read overflow  ***********************/
    printf("Case8 started\n");
    ret = w25q64flash_read_data(spiflash, operate_addr, out, 256);
    ASSERT_TRUE(ret != 0);

    /**************** Preparing data   *****************************/
    free(data);
    free(out);

    operate_addr = SPIFLASH_BASE_ADDR + SECTOR_SIZE - 0x8;
    data = (char *)malloc(256);
    memset(data, 0x2, 256);
    out = (char *)malloc(256);

    /*****************  Erase sector  ************************/
    w25q64flash_erase_sector(spiflash, SPIFLASH_BASE_ADDR);
    w25q64flash_erase_sector(spiflash, SPIFLASH_BASE_ADDR + SECTOR_SIZE);

    /************* Case-9 Write over sector  *************************/
    printf("Case9 started\n");
    ret = w25q64flash_program_data(spiflash, operate_addr, data , 256);
    ASSERT_TRUE(ret == 256);

    /************* Case-10 Read over sector  *************************/
    ret = w25q64flash_read_data(spiflash, operate_addr, out, 256);
    ASSERT_TRUE(ret == 256);
    ASSERT_TRUE(memcmp(data, out, 256) == 0);

    free(data);
    free(out);

    ret = csi_spi_uninitialize(spiflash);

    printf("Test finished\n");
}

int test_spi(void)
{
    dtest_suite_info_t test_suite_info = {
        "test_spi", NULL, NULL, NULL, NULL
    };

    dtest_suite_t *test_suite = dtest_add_suite(&test_suite_info);

    dtest_case_info_t test_case_info_array[] = {
        {"spi_if_test", spi_if_test, 1},
        {"spi_func_test", spi_func_test, 1},
        {"spi_func_test2", spi_func_test2, 1},
        {NULL, NULL, 0}
    };

    dtest_add_cases(test_suite, test_case_info_array);

    return 0;
}

int main(void)
{
    dtest_register_cb(test_spi);
    dtest();

    return 0;
}
