/*
 * Copyright (C) 2017-2019 Alibaba Group Holding Limited
 */

/******************************************************************************
 * @file     spi_speed.c
 * @brief    spi speed test
 * @version  V1.0
 * @date     14. Jun 2019
 ******************************************************************************/

#include "spi_test1.h"
#include "stdio.h"
#include <string.h>
#include <stdlib.h>
#include "timer_port.h"

#define DATA_LENGTH 2048

static uint8_t send_buffer[DATA_LENGTH] = {0x0};
static uint8_t receive_buffer[DATA_LENGTH] = {0x0};
static volatile uint8_t lock = 0;
static unsigned long long time_num = 0;

void spi_callback(int32_t idx, spi_event_e event)
{
    //printf("***%d\n",idx);
    if (idx == SPI_INDEX) {
        if (event == SPI_EVENT_TRANSFER_COMPLETE) {
            time_num = clock_gettime();
            lock = 0;
        }
    }

}

void spi_speed_test()
{
    int32_t ret;
    spi_handle_t spi;
    unsigned long long time_start = 0, time_end = 0;
    uint32_t i;

    csi_pin_set_mux(SPI_TX, SPI_TX_FUNC);
    csi_pin_set_mux(SPI_RX, SPI_RX_FUNC);
    csi_pin_set_mux(SPI_CLK, SPI_CLK_FUNC);
    csi_pin_set_mux(SPI_CS, SPI_CS_FUNC);

    //init data
    for (i = 0; i < DATA_LENGTH; i++) {
        send_buffer[i] = rand();
    }

    //spi = csi_spi_initialize(SPI_INDEX, spi_callback);
    spi = csi_spi_initialize(SPI_INDEX, NULL);
    ASSERT_TRUE(spi != NULL);

    ret = csi_spi_config_baudrate(spi, 400);
    ASSERT_TRUE(ret == 0);
    ret = csi_spi_config_mode(spi, SPI_MODE_MASTER);
    ASSERT_TRUE(ret == 0);
    ret = csi_spi_config_format(spi, SPI_FORMAT_CPOL0_CPHA0);
    ASSERT_TRUE(ret == 0);
    ret = csi_spi_config_bit_order(spi, SPI_ORDER_MSB2LSB);
    ASSERT_TRUE(ret == 0);
    ret = csi_spi_config_ss_mode(spi, SPI_SS_MASTER_SW);
    ASSERT_TRUE(ret == 0);
    ret = csi_spi_config_datawidth(spi, 8);
    ASSERT_TRUE(ret == 0);

    ret = csi_spi_config_block_mode(spi, 1);
    ASSERT_TRUE(ret == 0);

    //printf("hello\n");
    Timer_Open();
    lock = 1;
    Timer_Start();
    time_start = clock_gettime();
    ret = csi_spi_transfer(spi, &send_buffer[0], &receive_buffer[0], DATA_LENGTH, DATA_LENGTH);
    ASSERT_TRUE(ret == 0);

    spi_status_t sta;
    sta = csi_spi_get_status(spi);

    while (sta.busy) {
        sta = csi_spi_get_status(spi);
    }

    time_num = clock_gettime();

//	while (lock) ;
    time_end = time_num;

    if (strncmp((const char *)&receive_buffer[0], (const char *)&send_buffer[0], DATA_LENGTH) != 0) {
        printf("spi transfer error!!!\n");
    }

    printf("transfer size:  %d Bytes\ntransfer time: start is %lld us, end is %lld us\n", DATA_LENGTH, time_start, time_end);
    printf("\ntransfer speed:   %lld Bytes/s\n", (unsigned long long)DATA_LENGTH * 1000000 / (time_end - time_start));

    ret = csi_spi_uninitialize(spi);
    ASSERT_TRUE(ret == 0);

    Timer_Stop();
    Timer_Close();
}
