/*
 * Copyright (C) 2017-2019 Alibaba Group Holding Limited
 */

/******************************************************************************
 * @file     sdhc.h
 * @brief    header file for mmc driver
 * @version  V1.0
 * @date     18. December 2019
 ******************************************************************************/

#ifndef _SDHC_H_
#define _SDHC_H_

#include "sd_cmds.h"
#include "mmc_dev.h"
#include "mmc_core.h"
#include "sdhci.h"
#include "mmcm_clk.h"
#include "sd_cmd.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    struct sdhci_t *sdhci;
    struct mmc_dev_t *emmc_dev;
    struct sd_card_t sd_card;
}dev_mmc_t;

typedef struct {
    uint32_t speed_mode;    ///<mmc devices speed mode
    /*
        #define XEMMC_DS_SPEED_MODE	0
	    #define XEMMC_HS_SPEED_MODE	1
	    #define XEMMC_HS200_SPEED_MODE	2
	    #define XEMMC_HS400_SPEED_MODE	3
	    #define XEMMC_HSDDR_SPEED_MODE	4
        //for sd mode
        #define	SDR12_DS_SPEED_MODE	0
	    #define	SDR25_HS_SPEED_MODE	1
	    #define	SDR50_SPEED_MODE	2
	    #define	SDR104_SPEED_MODE	3
	    #define DDR50_SPEED_MODE	4
    */
    uint32_t vdd;           ///<mmc devices bus voltage
    /*
        #define XVDD1_3P3_VOLT		0
        #define XVDD1_1P8_VOLT		1
        #define XVDD1_3P0_VOLT		2
        #define XVDD1_1P2_VOLT		3
        #define XVDD1_POWER_OFF		4
     */
    uint32_t bus_width;     ///<mmc devices bus width
    /*
        #define	EMMC_SET_BUSWIDTH_1BIT		0
	    #define	EMMC_SET_BUSWIDTH_4BIT		1
	    #define	EMMC_SET_BUSWIDTH_8BIT		2
	    #define	EMMC_SET_BUSWIDTH_4BIT_DDR	5
	    #define	EMMC_SET_BUSWIDTH_8BIT_DDR	6
        //for sd mode
        #define SD_BUSWIDTH_1		0
        #define SD_BUSWIDTH_4		1
        #define SD_BUSWIDTH_8		2
     */
    uint32_t clk_freq;      ///<mmc devices bus clock freq
    uint16_t rca;  /*card id, Non-zero value*/
} dev_mmc_config_t;

typedef enum {
    EMMC_BOOT_ENABLE_NONE = 0,
    EMMC_BOOT_ENABLE_PARTITION_1 = 1,
    EMMC_BOOT_ENABLE_PARTITION_2 = 2,
    EMMC_USER_ENABLE_AREA_FOR_BOOT = 7,
} emmc_partition_en;

typedef enum {
    EMMC_BOOT_ACCESS_NONE = 0,
    EMMC_BOOT_ACCESS_PARTITION_1 = 1,
    EMMC_BOOT_ACCESS_PARTITION_2 = 2,
    EMMC_ACCESS_RPMB = 3,
} emmc_partition_access;

typedef enum {
    EMMC_BOOT_BUS_1BIT= 0,
    EMMC_BOOT_BUS_4BIT= 1,
    EMMC_BOOT_BUS_8BIT= 2,
} emmc_boot_bus_width;

typedef struct {
    uint8_t bus_width;
    uint16_t block_cnt;
} emmc_boot_info_t;



/**
  \brief       Initializes the resources needed for the mmc interface.
  \param[in]   handler Operate handle.
  \param[in]   idx       The device idx.
  \return      0 ok, other error.
*/
int dev_mmc_init(dev_mmc_t *handler, int idx);

/**
  \brief       Config the host controller.
  \param[in]   handler Operate handle.
  \param[in]   config  the devices config param.
  \return      0 ok, other error.
*/
int dev_mmc_config(dev_mmc_t *handler, dev_mmc_config_t *config);

/**
  \brief       Enable the emmc boot partition.
  \            The boot area must be enabled before the boot area can be accessed
  \param[in]   handler Operate handle.
  \param[in]   partition boot partition.
  \return      0 ok, other error.
*/
int dev_emmc_boot_partition_enable(dev_mmc_t *handler, emmc_partition_en partition);

/**
  \brief       Enable the emmc boot partition access.
  \            After enabling the boot partition and
  \            opening the boot partition access, you can read and write to the boot partition
  \param[in]   handler Operate handle.
  \param[in]   partition boot partition.
  \return      0 ok, other error.
*/
int dev_emmc_boot_partition_access(dev_mmc_t *handler, emmc_partition_access partition);

/**
  \brief       Set bus width for boot mode.
  \param[in]   handler Operate handle.
  \param[in]   width   bus width.
  \return      0 ok, other error.
*/
int dev_emmc_boot_set_bus_width(dev_mmc_t *handler, emmc_boot_bus_width width);

/**
  \brief       Get emmc boot partition info.
  \param[in]   handler Operate handle.
  \param[out]   info    boot partiton info.
  \return      0 ok, other error.
*/
int dev_emmc_boot_info(dev_mmc_t *handler, emmc_boot_info_t *info);

/**
  \brief       Emmc card config.
  \param[in]   handler Operate handle.
  \param[in]   config  the emmc card config param.
  \return      0 ok, other error.
*/
int dev_emmc_card_init(dev_mmc_t *handler, dev_mmc_config_t *config);

/**
 * \brief Is sd card inserted
 *	check whether card is inserted or not
 * \param none
 * \returns true if card is inserted, 0 otherwise
 */
int dev_sd_card_inserted(dev_mmc_t *handler);

/**
  \brief       SD card config.
  \param[in]   handler Operate handle.
  \param[in]   config  the SD card config param.
  \return      0 ok, other error.
*/
int dev_sd_card_init(dev_mmc_t *handler, dev_mmc_config_t *config);

/**
  \brief       Get sd card size.
  \param[in]   handler Operate handle.
  \return      -1 error, other sd card size.
*/
uint64_t dev_sd_card_size_kbytes(dev_mmc_t *handler);

/**
  \brief       mmc write.
  \param[in]   handler Operate handle.
  \param[in]   buf     write data.
  \param[in]   buf_len write data len.
  \param[in]   block_addr write addr.
  \param[in]   block_cnt  write block cnts.
  \return      0 ok, other error.
*/
uint32_t dev_mmc_write(dev_mmc_t *handler, uint8_t *buf, uint32_t buflen, uint32_t block_addr, uint32_t block_cnt);

/**
  \brief       mmc read.
  \param[in]   handler Operate handle.
  \param[in]   buf     write data.
  \param[in]   buf_len write data len.
  \param[in]   block_addr write addr.
  \param[in]   block_cnt  write block cnts.
  \return      0 ok, other error.
*/
uint32_t dev_mmc_read(dev_mmc_t *handler, uint8_t *buf, uint32_t buflen, uint32_t block_addr, uint32_t block_cnt);


/**
  \brief       De-initialize mmc Interface.
  \param[in]   handler Operate handle.
  \return      none.
*/
void dev_mmc_uninit(dev_mmc_t *handler);

#endif
