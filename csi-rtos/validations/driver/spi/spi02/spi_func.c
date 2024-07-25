/*
 * Copyright (C) 2017-2019 Alibaba Group Holding Limited
 */

/******************************************************************************
 * @file     spi_func.c
 * @brief    spi func2 test
 * @version  V1.0
 * @date     14. Jun 2019
 ******************************************************************************/

#include "spi_test1.h"
#include "stdio.h"
#include <string.h>

static void spi_event_cb_fun(int32_t idx, spi_event_e event)
{
    printf("SPI event entered!\n");
}

static uint8_t msg_buffer[4096] = {0x0};
static const char *msg = "a!\"#$%&'()*+,-.0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~i0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ\n\0";

static uint16_t msg1[1024] = {0x0};
static uint32_t msg2[1024] = {0x0};

static void clear_buffer(uint8_t *buffer, uint32_t begin, uint32_t end)
{
    uint32_t i;

    for (i = begin; i < end; i++) {
        *(buffer + i) = 0x0;
    }

}

static spi_config_t config[] = {
    {1000000, SPI_MODE_MASTER, SPI_FORMAT_CPOL0_CPHA0, SPI_ORDER_MSB2LSB, SPI_SS_MASTER_SW, 8},
    {2000000, SPI_MODE_MASTER, SPI_FORMAT_CPOL0_CPHA0, SPI_ORDER_MSB2LSB, SPI_SS_MASTER_HW_OUTPUT, 8},
    {2000000, SPI_MODE_MASTER, SPI_FORMAT_CPOL0_CPHA0, SPI_ORDER_LSB2MSB, SPI_SS_MASTER_SW, 8},
    {2000000, SPI_MODE_MASTER, SPI_FORMAT_CPOL0_CPHA0, SPI_ORDER_LSB2MSB, SPI_SS_MASTER_HW_OUTPUT, 8},

    {2000000, SPI_MODE_MASTER, SPI_FORMAT_CPOL0_CPHA1, SPI_ORDER_MSB2LSB, SPI_SS_MASTER_SW, 8},
    {2000000, SPI_MODE_MASTER, SPI_FORMAT_CPOL0_CPHA1, SPI_ORDER_MSB2LSB, SPI_SS_MASTER_HW_OUTPUT, 8},
    {2000000, SPI_MODE_MASTER, SPI_FORMAT_CPOL0_CPHA1, SPI_ORDER_LSB2MSB, SPI_SS_MASTER_SW, 8},
    {2000000, SPI_MODE_MASTER, SPI_FORMAT_CPOL0_CPHA1, SPI_ORDER_LSB2MSB, SPI_SS_MASTER_HW_OUTPUT, 8},

    {2000000, SPI_MODE_MASTER, SPI_FORMAT_CPOL1_CPHA0, SPI_ORDER_MSB2LSB, SPI_SS_MASTER_SW, 8},
    {2000000, SPI_MODE_MASTER, SPI_FORMAT_CPOL1_CPHA0, SPI_ORDER_MSB2LSB, SPI_SS_MASTER_HW_OUTPUT, 8},
    {2000000, SPI_MODE_MASTER, SPI_FORMAT_CPOL1_CPHA0, SPI_ORDER_LSB2MSB, SPI_SS_MASTER_SW, 8},
    {2000000, SPI_MODE_MASTER, SPI_FORMAT_CPOL1_CPHA0, SPI_ORDER_LSB2MSB, SPI_SS_MASTER_HW_OUTPUT, 8},

    {2000000, SPI_MODE_MASTER, SPI_FORMAT_CPOL1_CPHA1, SPI_ORDER_MSB2LSB, SPI_SS_MASTER_SW, 8},
    {2000000, SPI_MODE_MASTER, SPI_FORMAT_CPOL1_CPHA1, SPI_ORDER_MSB2LSB, SPI_SS_MASTER_HW_OUTPUT, 8},
    {2000000, SPI_MODE_MASTER, SPI_FORMAT_CPOL1_CPHA1, SPI_ORDER_LSB2MSB, SPI_SS_MASTER_SW, 8},
    {2000000, SPI_MODE_MASTER, SPI_FORMAT_CPOL1_CPHA1, SPI_ORDER_LSB2MSB, SPI_SS_MASTER_HW_OUTPUT, 8},
    /*****************************************/

    {20000000, SPI_MODE_MASTER, SPI_FORMAT_CPOL0_CPHA0, SPI_ORDER_MSB2LSB, SPI_SS_MASTER_SW, 8},
    {20000000, SPI_MODE_MASTER, SPI_FORMAT_CPOL0_CPHA0, SPI_ORDER_MSB2LSB, SPI_SS_MASTER_HW_OUTPUT, 8},
    {20000000, SPI_MODE_MASTER, SPI_FORMAT_CPOL0_CPHA0, SPI_ORDER_LSB2MSB, SPI_SS_MASTER_SW, 8},
    {20000000, SPI_MODE_MASTER, SPI_FORMAT_CPOL0_CPHA0, SPI_ORDER_LSB2MSB, SPI_SS_MASTER_HW_OUTPUT, 8},

    {20000000, SPI_MODE_MASTER, SPI_FORMAT_CPOL0_CPHA1, SPI_ORDER_MSB2LSB, SPI_SS_MASTER_SW, 8},
    {20000000, SPI_MODE_MASTER, SPI_FORMAT_CPOL0_CPHA1, SPI_ORDER_MSB2LSB, SPI_SS_MASTER_HW_OUTPUT, 8},
    {20000000, SPI_MODE_MASTER, SPI_FORMAT_CPOL0_CPHA1, SPI_ORDER_LSB2MSB, SPI_SS_MASTER_SW, 8},
    {20000000, SPI_MODE_MASTER, SPI_FORMAT_CPOL0_CPHA1, SPI_ORDER_LSB2MSB, SPI_SS_MASTER_HW_OUTPUT, 8},

    {20000000, SPI_MODE_MASTER, SPI_FORMAT_CPOL1_CPHA0, SPI_ORDER_MSB2LSB, SPI_SS_MASTER_SW, 8},
    {20000000, SPI_MODE_MASTER, SPI_FORMAT_CPOL1_CPHA0, SPI_ORDER_MSB2LSB, SPI_SS_MASTER_HW_OUTPUT, 8},
    {20000000, SPI_MODE_MASTER, SPI_FORMAT_CPOL1_CPHA0, SPI_ORDER_LSB2MSB, SPI_SS_MASTER_SW, 8},
    {20000000, SPI_MODE_MASTER, SPI_FORMAT_CPOL1_CPHA0, SPI_ORDER_LSB2MSB, SPI_SS_MASTER_HW_OUTPUT, 8},

    {20000000, SPI_MODE_MASTER, SPI_FORMAT_CPOL1_CPHA1, SPI_ORDER_MSB2LSB, SPI_SS_MASTER_SW, 8},
    {20000000, SPI_MODE_MASTER, SPI_FORMAT_CPOL1_CPHA1, SPI_ORDER_MSB2LSB, SPI_SS_MASTER_HW_OUTPUT, 8},
    {20000000, SPI_MODE_MASTER, SPI_FORMAT_CPOL1_CPHA1, SPI_ORDER_LSB2MSB, SPI_SS_MASTER_SW, 8},
    {20000000, SPI_MODE_MASTER, SPI_FORMAT_CPOL1_CPHA1, SPI_ORDER_LSB2MSB, SPI_SS_MASTER_HW_OUTPUT, 8},
    /***********************************************************/

    {2000000, SPI_MODE_MASTER, SPI_FORMAT_CPOL0_CPHA0, SPI_ORDER_MSB2LSB, SPI_SS_MASTER_SW, 16},
    {2000000, SPI_MODE_MASTER, SPI_FORMAT_CPOL0_CPHA0, SPI_ORDER_MSB2LSB, SPI_SS_MASTER_HW_OUTPUT, 16},
    {2000000, SPI_MODE_MASTER, SPI_FORMAT_CPOL0_CPHA0, SPI_ORDER_LSB2MSB, SPI_SS_MASTER_SW, 16},
    {2000000, SPI_MODE_MASTER, SPI_FORMAT_CPOL0_CPHA0, SPI_ORDER_LSB2MSB, SPI_SS_MASTER_HW_OUTPUT, 16},

    {2000000, SPI_MODE_MASTER, SPI_FORMAT_CPOL0_CPHA1, SPI_ORDER_MSB2LSB, SPI_SS_MASTER_SW, 16},
    {2000000, SPI_MODE_MASTER, SPI_FORMAT_CPOL0_CPHA1, SPI_ORDER_MSB2LSB, SPI_SS_MASTER_HW_OUTPUT, 16},
    {2000000, SPI_MODE_MASTER, SPI_FORMAT_CPOL0_CPHA1, SPI_ORDER_LSB2MSB, SPI_SS_MASTER_SW, 16},
    {2000000, SPI_MODE_MASTER, SPI_FORMAT_CPOL0_CPHA1, SPI_ORDER_LSB2MSB, SPI_SS_MASTER_HW_OUTPUT, 16},

    {2000000, SPI_MODE_MASTER, SPI_FORMAT_CPOL1_CPHA0, SPI_ORDER_MSB2LSB, SPI_SS_MASTER_SW, 16},
    {2000000, SPI_MODE_MASTER, SPI_FORMAT_CPOL1_CPHA0, SPI_ORDER_MSB2LSB, SPI_SS_MASTER_HW_OUTPUT, 16},
    {2000000, SPI_MODE_MASTER, SPI_FORMAT_CPOL1_CPHA0, SPI_ORDER_LSB2MSB, SPI_SS_MASTER_SW, 16},
    {2000000, SPI_MODE_MASTER, SPI_FORMAT_CPOL1_CPHA0, SPI_ORDER_LSB2MSB, SPI_SS_MASTER_HW_OUTPUT, 16},

    {2000000, SPI_MODE_MASTER, SPI_FORMAT_CPOL1_CPHA1, SPI_ORDER_MSB2LSB, SPI_SS_MASTER_SW, 16},
    {2000000, SPI_MODE_MASTER, SPI_FORMAT_CPOL1_CPHA1, SPI_ORDER_MSB2LSB, SPI_SS_MASTER_HW_OUTPUT, 16},
    {2000000, SPI_MODE_MASTER, SPI_FORMAT_CPOL1_CPHA1, SPI_ORDER_LSB2MSB, SPI_SS_MASTER_SW, 16},
    {2000000, SPI_MODE_MASTER, SPI_FORMAT_CPOL1_CPHA1, SPI_ORDER_LSB2MSB, SPI_SS_MASTER_HW_OUTPUT, 16},
    /*****************************************/

    {20000000, SPI_MODE_MASTER, SPI_FORMAT_CPOL0_CPHA0, SPI_ORDER_MSB2LSB, SPI_SS_MASTER_SW, 16},
    {20000000, SPI_MODE_MASTER, SPI_FORMAT_CPOL0_CPHA0, SPI_ORDER_MSB2LSB, SPI_SS_MASTER_HW_OUTPUT, 16},
    {20000000, SPI_MODE_MASTER, SPI_FORMAT_CPOL0_CPHA0, SPI_ORDER_LSB2MSB, SPI_SS_MASTER_SW, 16},
    {20000000, SPI_MODE_MASTER, SPI_FORMAT_CPOL0_CPHA0, SPI_ORDER_LSB2MSB, SPI_SS_MASTER_HW_OUTPUT, 16},

    {20000000, SPI_MODE_MASTER, SPI_FORMAT_CPOL0_CPHA1, SPI_ORDER_MSB2LSB, SPI_SS_MASTER_SW, 16},
    {20000000, SPI_MODE_MASTER, SPI_FORMAT_CPOL0_CPHA1, SPI_ORDER_MSB2LSB, SPI_SS_MASTER_HW_OUTPUT, 16},
    {20000000, SPI_MODE_MASTER, SPI_FORMAT_CPOL0_CPHA1, SPI_ORDER_LSB2MSB, SPI_SS_MASTER_SW, 16},
    {20000000, SPI_MODE_MASTER, SPI_FORMAT_CPOL0_CPHA1, SPI_ORDER_LSB2MSB, SPI_SS_MASTER_HW_OUTPUT, 16},

    {20000000, SPI_MODE_MASTER, SPI_FORMAT_CPOL1_CPHA0, SPI_ORDER_MSB2LSB, SPI_SS_MASTER_SW, 16},
    {20000000, SPI_MODE_MASTER, SPI_FORMAT_CPOL1_CPHA0, SPI_ORDER_MSB2LSB, SPI_SS_MASTER_HW_OUTPUT, 16},
    {20000000, SPI_MODE_MASTER, SPI_FORMAT_CPOL1_CPHA0, SPI_ORDER_LSB2MSB, SPI_SS_MASTER_SW, 16},
    {20000000, SPI_MODE_MASTER, SPI_FORMAT_CPOL1_CPHA0, SPI_ORDER_LSB2MSB, SPI_SS_MASTER_HW_OUTPUT, 16},

    {20000000, SPI_MODE_MASTER, SPI_FORMAT_CPOL1_CPHA1, SPI_ORDER_MSB2LSB, SPI_SS_MASTER_SW, 16},
    {20000000, SPI_MODE_MASTER, SPI_FORMAT_CPOL1_CPHA1, SPI_ORDER_MSB2LSB, SPI_SS_MASTER_HW_OUTPUT, 16},
    {20000000, SPI_MODE_MASTER, SPI_FORMAT_CPOL1_CPHA1, SPI_ORDER_LSB2MSB, SPI_SS_MASTER_SW, 16},
    {20000000, SPI_MODE_MASTER, SPI_FORMAT_CPOL1_CPHA1, SPI_ORDER_LSB2MSB, SPI_SS_MASTER_HW_OUTPUT, 16},
    /*****************************/

    {2000000, SPI_MODE_MASTER, SPI_FORMAT_CPOL0_CPHA0, SPI_ORDER_MSB2LSB, SPI_SS_MASTER_SW, 32},
    {2000000, SPI_MODE_MASTER, SPI_FORMAT_CPOL0_CPHA0, SPI_ORDER_MSB2LSB, SPI_SS_MASTER_HW_OUTPUT, 32},
    {2000000, SPI_MODE_MASTER, SPI_FORMAT_CPOL0_CPHA0, SPI_ORDER_LSB2MSB, SPI_SS_MASTER_SW, 32},
    {2000000, SPI_MODE_MASTER, SPI_FORMAT_CPOL0_CPHA0, SPI_ORDER_LSB2MSB, SPI_SS_MASTER_HW_OUTPUT, 32},

    {2000000, SPI_MODE_MASTER, SPI_FORMAT_CPOL0_CPHA1, SPI_ORDER_MSB2LSB, SPI_SS_MASTER_SW, 32},
    {2000000, SPI_MODE_MASTER, SPI_FORMAT_CPOL0_CPHA1, SPI_ORDER_MSB2LSB, SPI_SS_MASTER_HW_OUTPUT, 32},
    {2000000, SPI_MODE_MASTER, SPI_FORMAT_CPOL0_CPHA1, SPI_ORDER_LSB2MSB, SPI_SS_MASTER_SW, 32},
    {2000000, SPI_MODE_MASTER, SPI_FORMAT_CPOL0_CPHA1, SPI_ORDER_LSB2MSB, SPI_SS_MASTER_HW_OUTPUT, 32},

    {2000000, SPI_MODE_MASTER, SPI_FORMAT_CPOL1_CPHA0, SPI_ORDER_MSB2LSB, SPI_SS_MASTER_SW, 32},
    {2000000, SPI_MODE_MASTER, SPI_FORMAT_CPOL1_CPHA0, SPI_ORDER_MSB2LSB, SPI_SS_MASTER_HW_OUTPUT, 32},
    {2000000, SPI_MODE_MASTER, SPI_FORMAT_CPOL1_CPHA0, SPI_ORDER_LSB2MSB, SPI_SS_MASTER_SW, 32},
    {2000000, SPI_MODE_MASTER, SPI_FORMAT_CPOL1_CPHA0, SPI_ORDER_LSB2MSB, SPI_SS_MASTER_HW_OUTPUT, 32},

    {2000000, SPI_MODE_MASTER, SPI_FORMAT_CPOL1_CPHA1, SPI_ORDER_MSB2LSB, SPI_SS_MASTER_SW, 32},
    {2000000, SPI_MODE_MASTER, SPI_FORMAT_CPOL1_CPHA1, SPI_ORDER_MSB2LSB, SPI_SS_MASTER_HW_OUTPUT, 32},
    {2000000, SPI_MODE_MASTER, SPI_FORMAT_CPOL1_CPHA1, SPI_ORDER_LSB2MSB, SPI_SS_MASTER_SW, 32},
    {2000000, SPI_MODE_MASTER, SPI_FORMAT_CPOL1_CPHA1, SPI_ORDER_LSB2MSB, SPI_SS_MASTER_HW_OUTPUT, 32},
    /***************************************/

    {20000000, SPI_MODE_MASTER, SPI_FORMAT_CPOL0_CPHA0, SPI_ORDER_MSB2LSB, SPI_SS_MASTER_SW, 32},
    {20000000, SPI_MODE_MASTER, SPI_FORMAT_CPOL0_CPHA0, SPI_ORDER_MSB2LSB, SPI_SS_MASTER_HW_OUTPUT, 32},
    {20000000, SPI_MODE_MASTER, SPI_FORMAT_CPOL0_CPHA0, SPI_ORDER_LSB2MSB, SPI_SS_MASTER_SW, 32},
    {20000000, SPI_MODE_MASTER, SPI_FORMAT_CPOL0_CPHA0, SPI_ORDER_LSB2MSB, SPI_SS_MASTER_HW_OUTPUT, 32},

    {20000000, SPI_MODE_MASTER, SPI_FORMAT_CPOL0_CPHA1, SPI_ORDER_MSB2LSB, SPI_SS_MASTER_SW, 32},
    {20000000, SPI_MODE_MASTER, SPI_FORMAT_CPOL0_CPHA1, SPI_ORDER_MSB2LSB, SPI_SS_MASTER_HW_OUTPUT, 32},
    {20000000, SPI_MODE_MASTER, SPI_FORMAT_CPOL0_CPHA1, SPI_ORDER_LSB2MSB, SPI_SS_MASTER_SW, 32},
    {20000000, SPI_MODE_MASTER, SPI_FORMAT_CPOL0_CPHA1, SPI_ORDER_LSB2MSB, SPI_SS_MASTER_HW_OUTPUT, 32},

    {20000000, SPI_MODE_MASTER, SPI_FORMAT_CPOL1_CPHA0, SPI_ORDER_MSB2LSB, SPI_SS_MASTER_SW, 32},
    {20000000, SPI_MODE_MASTER, SPI_FORMAT_CPOL1_CPHA0, SPI_ORDER_MSB2LSB, SPI_SS_MASTER_HW_OUTPUT, 32},
    {20000000, SPI_MODE_MASTER, SPI_FORMAT_CPOL1_CPHA0, SPI_ORDER_LSB2MSB, SPI_SS_MASTER_SW, 32},
    {20000000, SPI_MODE_MASTER, SPI_FORMAT_CPOL1_CPHA0, SPI_ORDER_LSB2MSB, SPI_SS_MASTER_HW_OUTPUT, 32},

    {20000000, SPI_MODE_MASTER, SPI_FORMAT_CPOL1_CPHA1, SPI_ORDER_MSB2LSB, SPI_SS_MASTER_SW, 32},
    {20000000, SPI_MODE_MASTER, SPI_FORMAT_CPOL1_CPHA1, SPI_ORDER_MSB2LSB, SPI_SS_MASTER_HW_OUTPUT, 32},
    {20000000, SPI_MODE_MASTER, SPI_FORMAT_CPOL1_CPHA1, SPI_ORDER_LSB2MSB, SPI_SS_MASTER_SW, 32},
    {20000000, SPI_MODE_MASTER, SPI_FORMAT_CPOL1_CPHA1, SPI_ORDER_LSB2MSB, SPI_SS_MASTER_HW_OUTPUT, 32},

};

void spi_func_test2()
{
    int32_t ret;
    spi_handle_t spi;
    int cmp;

    csi_pin_set_mux(SPI_TX, SPI_TX_FUNC);
    csi_pin_set_mux(SPI_RX, SPI_RX_FUNC);
    csi_pin_set_mux(SPI_CLK, SPI_CLK_FUNC);
    csi_pin_set_mux(SPI_CS, SPI_CS_FUNC);
    /*
     *	CM6501's spi dot supported interrupt mode
     */
    spi = csi_spi_initialize(SPI_INDEX, NULL);
    ASSERT_TRUE(spi != NULL);

//spi config
    /*
    	ret = csi_spi_config_baudrate(spi,2000000);
    	ASSERT_TRUE(ret == 0);
    	ret = csi_spi_config_mode(spi,SPI_MODE_MASTER);
    	ASSERT_TRUE(ret == 0);
    	ret = csi_spi_config_format(spi,SPI_FORMAT_CPOL0_CPHA0);
    	ASSERT_TRUE(ret == 0);
    	ret = csi_spi_config_bit_order(spi,SPI_ORDER_MSB2LSB);
    	ret = csi_spi_config_ss_mode(spi,SPI_SS_MASTER_SW);
    	ASSERT_TRUE(ret == 0);
    	ret = csi_spi_config_datawidth(spi,8);
    	ASSERT_TRUE(ret == 0);
    */
    for (uint16_t i = 0; i < sizeof(config) / sizeof(spi_config_t); i++) {
        //ret = csi_spi_config(spi, 2000000, SPI_MODE_MASTER, SPI_FORMAT_CPOL0_CPHA0, SPI_ORDER_MSB2LSB, SPI_SS_MASTER_SW, 8);
        //printf("ret = %d\n",ret);
        /*	ret = csi_spi_config(spi,config[i].baud,config[i].mode,config[i].format,config[i].order,config[i].ss_mode,config[i].bit_width );
        	printf("+++++%d\n",i);
        	if (!ret)
        		printf("config error -- %d\n",i);
        	else {
        */

        if ((config[i].order == SPI_ORDER_LSB2MSB) || (config[i].bit_width != 8)) {
            continue;
        } else {
            printf("+++++%d\n", i);

            ret = csi_spi_config_baudrate(spi, config[i].baud);
            ASSERT_TRUE(ret == 0);
            ret = csi_spi_config_mode(spi, config[i].mode);
            ASSERT_TRUE(ret == 0);
            ret = csi_spi_config_format(spi, config[i].format);
            ASSERT_TRUE(ret == 0);
            ret = csi_spi_config_bit_order(spi, config[i].order);
            ASSERT_TRUE(ret == 0);
            ret = csi_spi_config_ss_mode(spi, config[i].ss_mode);
            ASSERT_TRUE(ret == 0);
            ret = csi_spi_config_datawidth(spi, config[i].bit_width);
            ASSERT_TRUE(ret == 0);

            ret = csi_spi_config_block_mode(spi, 0);
            ASSERT_TRUE(ret == 0);

            /*
             *	CM6501 不支持中斷
             */
// test send and receive
#if 1

            if (config[i].bit_width == 8) {
                //ret = csi_spi_send(spi,msg,strlen(msg));
                //ASSERT_TRUE(ret == 0);
                ret = csi_spi_receive(spi, &msg_buffer[0], strlen(msg));
                //printf("*****%d\n",ret);
                ASSERT_TRUE(ret == 0);
                ret = csi_spi_send(spi, msg, strlen(msg));
                ASSERT_TRUE(ret == 0);

                spi_status_t status;
                uint8_t lock = 1;

                while (lock) {
                    status = csi_spi_get_status(spi);
                    printf("status:%d\n", status.busy);

                    if (status.busy != 1) {
                        lock = 0;
                    }

                    mdelay(1000);
                }

                cmp = strncmp(msg, (const char *)&msg_buffer[0], strlen(msg));
                ASSERT_TRUE(cmp == 0);
            }

#endif

            /*
             *	CM6501不支持16和32位
             */
// test transfer
#if 0

            if (config[i].bit_width == 8) {
                clear_buffer(&msg_buffer[0], 0, 4095);


                ret = csi_spi_transfer(spi, msg, &msg_buffer[2048], strlen(msg), strlen(msg));
                ASSERT_TRUE(ret == 0);

                spi_status_t status1;
                uint8_t lock1 = 1;

                while (lock1) {
                    status1 = csi_spi_get_status(spi);

                    if (status1.busy != 1) {
                        lock1 = 0;
                    }

                    mdelay(1000);
                }

                cmp = strncmp(msg, (const char *)&msg_buffer[2048], 10);
                ASSERT_TRUE(cmp == 0);
            }

            if (config[i].bit_width == 16) {
                clear_buffer(&msg_buffer[0], 0, 4095);
                uint32_t j;

                for (j = 0; j < 1024; j++) {
                    msg1[j] = (j % 256) * 256 + 255;
                }

                ret = csi_spi_transfer(spi, msg1, &msg_buffer[0], 1024, 1024);
                ASSERT_TRUE(ret == 0);

                spi_status_t status2;
                uint8_t lock2 = 1;

                while (lock2) {
                    status2 = csi_spi_get_status(spi);

                    if (status2.busy != 1) {
                        lock2 = 0;
                    }

                    mdelay(1000);
                }

                for (j = 0; j < 2048;) {
                    ASSERT_TRUE(msg_buffer[j] == j % 256);
                    ASSERT_TRUE(msg_buffer[j + 1] == 0xff);
                    j += 2;
                }
            }

            if (config[i].bit_width == 32) {
                clear_buffer(&msg_buffer[0], 0, 4095);
                uint32_t k;

                for (k = 0; k < 1024; k++) {
                    msg2[k] = 65535 + (k % 256) * 65536;
                }

                ret = csi_spi_transfer(spi, msg2, &msg_buffer[0], 1024, 1024);
                ASSERT_TRUE(ret == 0);

                spi_status_t status3;
                uint8_t lock3 = 1;

                while (lock3) {
                    status3 = csi_spi_get_status(spi);

                    if (status3.busy != 1) {
                        lock3 = 0;
                    }

                    mdelay(1000);
                }

                for (k = 0; k < 4096;) {
                    ASSERT_TRUE(msg_buffer[k] == 0);
                    ASSERT_TRUE(msg_buffer[k + 1] == k % 256);
                    ASSERT_TRUE(msg_buffer[k + 2] == 0xff);
                    ASSERT_TRUE(msg_buffer[k + 3] == 0xff);
                    k += 4;
                }

            }

#endif

// test abort transfer
#if 0

            if (config[i].bit_width == 8) {
                clear_buffer(&msg_buffer[0], 0, 4095);

                msg_buffer[0] = 'y';
                ret = csi_spi_transfer(spi, msg, &msg_buffer[0], strlen(msg), strlen(msg));
                ASSERT_TRUE(ret == 0);
                ret = csi_spi_abort_transfer(spi);
                ASSERT_TRUE(ret == 0);

                spi_status_t status2;
                status2 = csi_spi_get_status(spi);
                ASSERT_TRUE(status2.busy == 0);

                mdelay(100);
                cmp = strncmp(msg, (const char *)&msg_buffer[0], strlen(msg));
                ASSERT_TRUE(cmp != 0);
                //ASSERT_TRUE(msg_buffer[0] == 'a');
            }

#endif

        }
    }

    ret = csi_spi_uninitialize(spi);
    ASSERT_TRUE(ret == 0);

}
