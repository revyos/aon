/*
 * Copyright (C) 2017-2020 Alibaba Group Holding Limited
 */

/******************************************************************************
 * @file       ricoh567.h
 * @brief      Header File for ricoh567
 * @version    V1.0
 * @date       9. Nov 2021
 * @model      pmic
 ******************************************************************************/
#ifndef _RICOH567_H_
#define _RICOH567_H_

#define RICOH_WDT_EN            BIT(2)
#define RICOH_WDT_TIM_MSK       (0x03)
#define RICOH_WDT_SLP_EN        BIT(3)

#define RICOH_REG_WDT_CTRL      (0x0B)
#define RICOH_REG_WDT_CNT       (0x0C)

#define RICOH_PWR_OFF           BIT(0)
#define RICOH_PWR_SLEEP         BIT(4)
#define RICOH_PWR_EXIT_SLEEP    BIT(5)
#define RICOH_REG_PWR_SLP       (0x0E)

#define RICOH_PWR_REPWRON       BIT(0)
#define RICOH_PWR_REPWRTIM_MSK  (0x3 << 1)
#define RICOH_PWR_REPWRTIM_SHF  (1)
#define RICOH_PWR_RESETO_MSK    (0x3 << 4)
#define RICOH_PWR_RESETO_SHF    (4)
#define RICOH_REG_PWR_REPCT     (0x0F)


#define OFF_PRESS_TIM_POS       (0x4)
#define OFF_PRESS_TIME_MSK      (0x7 << OFF_PRESS_TIM_POS)
#define RICOH_REG_PWRONTIMSET   (0x10)

#define RICOH_REG_PWRIREN       (0x12)

#define RICOH_SLP_SLOT_MSK      (0xF)

#define RICOH_REG_DC1_SLOT      (0x16)
#define RICOH_REG_DC2_SLOT      (0x17)
#define RICOH_REG_DC3_SLOT      (0x18)
#define RICOH_REG_DC4_SLOT      (0x19)

#define RICOH_REG_LDO1_SLOT     (0x1B)
#define RICOH_REG_LDO2_SLOT     (0x1C)
#define RICOH_REG_LDO3_SLOT     (0x1D)
#define RICOH_REG_LDO4_SLOT     (0x1E)
#define RICOH_REG_LDO5_SLOT     (0x1F)

#define RICOH_REG_PSO0_SLOT     (0x25)
#define RICOH_REG_PSO1_SLOT     (0x26)
#define RICOH_REG_PSO2_SLOT     (0x27)
#define RICOH_REG_PSO3_SLOT     (0x28)



#define RICOH_DC_OUT_MASK       (0xff)

#define RICOH_REG_DC1_OUT       (0x36)
#define RICOH_REG_DC2_OUT       (0x37)
#define RICOH_REG_DC3_OUT       (0x38)
#define RICOH_REG_DC4_OUT       (0x39)

#define RICOH_LDO_OUT_MASK      (0x7f)

#define RICOH_REG_LDO1_OUT      (0x4c)
#define RICOH_REG_LDO2_OUT      (0x4d)
#define RICOH_REG_LDO3_OUT      (0x4e)
#define RICOH_REG_LDO4_OUT      (0x4f)
#define RICOH_REG_LDO5_OUT      (0x50)

#define RICOH_DC_EN             (1 << 0)

#define RICOH_REG_DC1_CTRL      (0x2c)
#define RICOH_REG_DC2_CTRL      (0x2e)
#define RICOH_REG_DC3_CTRL      (0x30)
#define RICOH_REG_DC4_CTRL      (0x32)

#define RICOH_LDO_EN_1         (1 << 0)
#define RICOH_LDO_EN_2         (1 << 1)
#define RICOH_LDO_EN_3         (1 << 2)
#define RICOH_LDO_EN_4         (1 << 3)
#define RICOH_LDO_EN_5         (1 << 4)

#define RICOH_REG_LDO_CTRL     (0x44)

#define RICOH_GPIO(x)          BIT(x)
#define RICOH_REG_IOSEL        (0x90)
#define RICOH_REG_IOOUT        (0x91)


typedef enum {
	RICOH567_ID_DC1,
	RICOH567_ID_DC2,
	RICOH567_ID_DC3,
	RICOH567_ID_DC4,
	RICOH567_ID_LDO1,
	RICOH567_ID_LDO2,
	RICOH567_ID_LDO3,
	RICOH567_ID_LDO4,
	RICOH567_ID_LDO5,
	RICOH567_ID_LDORTC1,
	RICOH567_ID_LDORTC2,
	RICOH567_ID_GPIO3,
}ricoh567_regu_hw_id_t;

#endif
