/*
 * Copyright (C) 2017-2019 Alibaba Group Holding Limited
 */

/******************************************************************************
 * @file     spi_test.h
 * @brief    the head file
 * @version  V1.0
 * @date     14. Jun 2019
 ******************************************************************************/

#include <drv/spi.h>
#include <stdlib.h>
#include "pin.h"
#include "pin_name.h"
#include "dtest.h"

#define SPIFLASH_BASE_ADDR    0x0
#define SECTOR_SIZE           0x8000
#define SECTOR_COUNT          0x2
#define SPIFLASH_ERASE_VALUE  0xff

//#define CSI_SPI_TX  PA17
//#define CSI_SPI_RX  PA18
//#define CSI_SPI_CLK  PA16
//#define CSI_SPI_CS   PA19

/*
#define SPI_INDEX   0
#define SPI_TX      GPIO11
#define SPI_RX      GPIO22
#define SPI_CLK     GPIO9
#define SPI_CS      GPIO20
#define SPI_TX_FUNC     GPIO11_SSP0_TXD
#define SPI_RX_FUNC     GPIO22_SSP0_RXD
#define SPI_CLK_FUNC    GPIO9_SSP0_CLK
#define SPI_CS_FUNC     GPIO20_SSP0_CS
*/
#define SPI_INDEX   0
#define SPI_TX      PB4
#define SPI_RX      PB5
#define SPI_CLK     PB6
#define SPI_CS      PB7
#define SPI_TX_FUNC     PB4_SPI1_MOSI
#define SPI_RX_FUNC     PB5_SPI1_MISO
#define SPI_CLK_FUNC    PB6_SPI1_CLK
#define SPI_CS_FUNC     PB7_SPI1_CS0

int32_t w25q64flash_read_id(const spi_handle_t *spi, uint32_t *id_num);
int32_t w25q64flash_erase_sector(const spi_handle_t *spi, uint32_t addr);
int32_t w25q64flash_erase_chip(const spi_handle_t *spi);
int32_t w25q64flash_program_data(const spi_handle_t *spi, uint32_t addr, const void *data, uint32_t cnt);
int32_t w25q64flash_read_data(const spi_handle_t *spi, uint32_t addr, void *data, uint32_t cnt);
