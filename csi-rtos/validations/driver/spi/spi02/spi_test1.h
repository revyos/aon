/*
 * Copyright (C) 2017-2019 Alibaba Group Holding Limited
 */

/******************************************************************************
 * @file     spi_test1.h
 * @brief    the head file
 * @version  V1.0
 * @date     14. Jun 2019
 ******************************************************************************/

#include <drv/spi.h>
#include <stdlib.h>
#include "pin.h"
#include <pin_name.h>
#include "dtest.h"

extern void mdelay(uint32_t ms);
void spi_func_test2(void);
void spi_speed_test(void);

typedef struct spi_config {
    int32_t baud;
    spi_mode_e mode;
    spi_format_e format;
    spi_bit_order_e order;
    spi_ss_mode_e ss_mode;
    int32_t	bit_width;
} spi_config_t;

//sc5654a
/*
#define SPI_INDEX	0
#define SPI_TX  	PB4
#define SPI_RX  	PB5
#define SPI_CLK  	PB6
#define SPI_CS   	PB7
#define SPI_TX_FUNC		PB4_SPI1_MOSI
#define SPI_RX_FUNC		PB5_SPI1_MISO
#define SPI_CLK_FUNC	PB6_SPI1_CLK
#define SPI_CS_FUNC		PB7_SPI1_CS0
*/

//cb2301
/*
#define SPI_INDEX   0
#define SPI_TX      PA17
#define SPI_RX      PA18
#define SPI_CLK     PA19
#define SPI_CS      PA16
#define SPI_TX_FUNC     PA17_SPI0_MOSI
#define SPI_RX_FUNC     PA18_SPI0_MISO
#define SPI_CLK_FUNC    PA19_SPI0_SCK
#define SPI_CS_FUNC     PIN_FUNC_GPIO
*/

//CB2201
#define SPI_INDEX   1
#define SPI_TX      PA23
#define SPI_RX      PA22
#define SPI_CLK     PA21
#define SPI_CS      PA18
#define SPI_TX_FUNC     PA23_SPI1_MOSI
#define SPI_RX_FUNC     PA22_SPI1_MISO
#define SPI_CLK_FUNC    PA21_SPI1_SCK
#define SPI_CS_FUNC     PA18_SPI1_CS0

//Danica
/*
#define SPI_INDEX   0
#define SPI_TX      PA25
#define SPI_RX      PA26
#define SPI_CLK     PA24
#define SPI_CS      PA23
#define SPI_TX_FUNC     PA25_SPI0_MOSI
#define SPI_RX_FUNC     PA26_SPI0_MISO
#define SPI_CLK_FUNC	PA24_SPI0_SCK
#define SPI_CS_FUNC     PA23_SPI0_CS
*/
