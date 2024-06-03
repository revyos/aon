/*
 * Copyright (C) 2017-2019 Alibaba Group Holding Limited
 */

#include "pin.h"
#define csi_spi_initialize              drv_spu_spi_initialize
#define csi_spi_uninitialize            drv_spu_spi_uninitialize
#define csi_spi_get_capabilities        drv_spu_spi_get_capabilities
#define csi_spi_config                  drv_spu_spi_config
#define csi_spi_send                    drv_spu_spi_send
#define csi_spi_receive                 drv_spu_spi_receive
#define csi_spi_transfer                drv_spu_spi_transfer
#define csi_spi_abort_transfer          drv_spu_spi_abort_transfer
#define csi_spi_get_status              drv_spu_spi_get_status
#define csi_spi_config_mode             drv_spu_spi_config_mode
#define csi_spi_config_block_mode       drv_spu_spi_config_block_mode
#define csi_spi_config_baudrate         drv_spu_spi_config_baudrate
#define csi_spi_config_bit_order        drv_spu_spi_config_bit_order
#define csi_spi_config_datawidth        drv_spu_spi_config_datawidth
#define csi_spi_config_format           drv_spu_spi_config_format
#define csi_spi_config_ss_mode          drv_spu_spi_config_ss_mode
#define csi_spi_get_data_count          drv_spu_spi_get_data_count
#define csi_spi_power_control           drv_spu_spi_power_control
#define csi_spi_ss_control              drv_spu_spi_ss_control
#define test_spi                        test_spu_spi
#define at25flash_read_data             spu_at25flash_read_data
#define at25flash_read_id               spu_at25flash_read_id
#define at25flash_erase_sector          spu_at25flash_erase_sector
#define at25flash_erase_chip            spu_at25flash_erase_chip
#define at25flash_program_data          spu_at25flash_program_data

#undef TEST_PIN_SPI_MISO
#undef TEST_PIN_SPI_MOSI
#undef TEST_PIN_SPI_CS
#undef TEST_PIN_SPI_SCK
#undef TEST_PIN_SPI_MISO_FUNC
#undef TEST_PIN_SPI_MOSI_FUNC
#undef TEST_PIN_SPI_CS_FUNC
#undef TEST_PIN_SPI_SCK_FUNC
#undef TEST_SPI_IDX

#define TEST_SPI_IDX                 TEST_SPU_SPI_IDX
#define TEST_PIN_SPI_MISO            TEST_PIN_SPU_SPI_MISO
#define TEST_PIN_SPI_MOSI            TEST_PIN_SPU_SPI_MOSI
#define TEST_PIN_SPI_CS              TEST_PIN_SPU_SPI_CS
#define TEST_PIN_SPI_SCK             TEST_PIN_SPU_SPI_SCK
#define TEST_PIN_SPI_MISO_FUNC       TEST_PIN_SPU_SPI_MISO_FUNC
#define TEST_PIN_SPI_MOSI_FUNC       TEST_PIN_SPU_SPI_MOSI_FUNC
#define TEST_PIN_SPI_CS_FUNC         TEST_PIN_SPU_SPI_CS_FUNC
#define TEST_PIN_SPI_SCK_FUNC        TEST_PIN_SPU_SPI_SCK_FUNC

#include "../../spi/spi01/w25q64fv.c"
#include "../../spi/spi01/test_spi.c"
