
/*
 * Copyright (C) 2017-2020 Alibaba Group Holding Limited
 */

/******************************************************************************
 * @file       da9063.h
 * @brief      Header File for da9063 
 * @version    V1.0
 * @date       9. Feb 2021
 * @model      pmic
 ******************************************************************************/
#ifndef _APTF110_H_
#define _APTF110_H_

typedef enum {
        WDOG_STOP  = 0x00,
        WDOG_START = 0x01,
        WDOG_FEED  = 0x02,
} wdg_flag_en;

#define PMIC_WDOG_TIMEOUT 4000 //ms
#define PMIC_REPOWER_DELAY 0   //ms

/* Pmic Control Registers */
#define APTF110_REG_ALL_REGU_POWER 0x00
#define APTF110_REG_REBOOT 0x01
#define APTF110_REG_REGU_POWER 0x02
#define APTF110_REG_REGU_VOL_SET 0x03
#define APTF110_REG_WDOG_CTRL 0x04
#define APTF110_REG_LOG_LEVEL 0x05
#define APTF110_REG_FAULTS_GET 0x06
#define PMIC_CTRL_REGU0_VOL 0x07
#define PMIC_CTRL_REGU1_VOL 0x08
#define PMIC_CTRL_REGU2_VOL 0x09
#define PMIC_CTRL_REGU3_VOL 0x0a


typedef enum {
	APTF110_ID_BASE,
	APTF110_ID_I2C_DC1,
	APTF110_ID_I2C_DC2,
	APTF110_ID_PGOOD_DC1,
	APTF110_ID_PGOOD_DC2,
	APTF110_ID_PGOOD_DC3,
	APTF110_ID_LDO1,
	APTF110_ID_LDO2,
	APTF110_ID_LDO3,
	APTF110_ID_LDO4,
	APTF110_ID_LDO5,
	APTF110_ID_LDO6,
	APTF110_ID_LDO7,
	APTF110_ID_LDO8,
	APTF110_ID_LDO9,
	APTF110_ID_LDO10,
	APTF110_ID_LDO11,
	APTF110_ID_LDO12,
	APTF110_ID_LDO13,
	APTF110_ID_LDO14,
}aptf110_regu_hw_id_t;

#endif